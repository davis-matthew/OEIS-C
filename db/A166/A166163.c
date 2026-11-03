/*
 * A166163  a(n) = the largest possible prime made by inserting either a 0 or
 *          a 1 anywhere in the binary representation of the n-th prime, then
 *          converting to decimal.
 * https://oeis.org/A166163
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the largest prime obtainable by inserting a 0 or a 1 into
 *            the binary representation of the n‑th prime, then interpreting
 *            the result as a binary number
 * Range:     u8 n <= 31, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 55 (2026-11-01)
 * Helpers:   is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/is_prime_seq.h"

/* Return the k‑th prime (k ≥ 1).  On first call the prime list
   is built incrementally using trial division.  The list is
   stored in a static array which is expanded as necessary.   */
static uint32_t get_prime(idx_t k)
{
    static uint32_t *pr = NULL;     /* array of primes */
    static size_t sz   = 0;         /* number of primes stored */
    static size_t cap  = 0;         /* allocated capacity */
    static uint32_t cand = 3;       /* candidate after 2 */

    /* initialise the prime table on first use */
    if (sz == 0) {
        cap = 8;
        pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
        if (!pr) abort();
        sz = 0;
    }

    /* grow the array until we have at least k primes */
    while (sz < (size_t)k) {
        if (sz == 0) {                /* add the first prime */
            pr[sz++] = 2;
            continue;
        }

        /* find the next prime candidate */
        uint32_t p = cand;
        bool ok = true;
        for (size_t i = 0; i < sz; ++i) {
            uint32_t prime = pr[i];
            if ((uint64_t)prime * prime > (uint64_t)p) break;
            if (p % prime == 0) { ok = false; break; }
        }

        if (!ok) {                    /* not prime – keep searching */
            cand += 2;                /* only odd numbers remain */
            continue;
        }

        /* prime found – store it, grow table if needed */
        if (sz == cap) {
            cap <<= 1;
            pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
            if (!pr) abort();
        }
        pr[sz++] = p;
        cand = p + 2;
    }

    return pr[(size_t)k - 1];
}

/* primitive prime test sufficient for the values produced ( < 1<<20 ) */
static inline bool is_prime(uint64_t v)
{
    return is_prime_seq(v);
}

seq_t a(idx_t n)
{
    /* 1‑based nth prime */
    uint32_t p = get_prime(n);

    /* binary representation of p */
    int len = 0;
    uint32_t tmp = p;
    while (tmp) { len++; tmp >>= 1; }

    /* store bits msb → lsb */
    uint8_t bits[64];
    tmp = p;
    for (int i = 0; i < len; ++i)
        bits[i] = (tmp >> (len - 1 - i)) & 1U;

    uint64_t best = 0;

    /* try all insertion points (0 … len) and digits 0/1 */
    for (int pos = 0; pos <= len; ++pos) {
        for (int ins = 0; ins <= 1; ++ins) {
            uint64_t cand = 0;
            for (int i = 0; i < len + 1; ++i) {
                cand <<= 1;
                if (i == pos)                /* inserted bit */
                    cand |= (uint64_t)ins;
                else                         /* original bit */
                    cand |= (uint64_t)bits[(i < pos) ? i : i - 1];
            }
            if (cand > 1 && is_prime(cand) && cand > best)
                best = cand;
        }
    }

    /* sequence definition guarantees at least one prime, but keep
       fallback to 0 in the unlikely event none found */
    return (seq_t)best;
}

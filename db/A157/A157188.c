/*
 * A157188  Number of ways to write prime(n) as p*q-(p+q) with primes p<=q.
 * https://oeis.org/A157188
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) counts pairs of primes p ≤ q with prime(n) = p*q-(p+q)
 * Range:     u8 n <= 54, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Validated: partial, pass, 105 terms through n = 105 (2026-11-01)
 * Helpers:   is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent to (p-1)(q-1) = prime(n)+1. */
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

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* out of range, but harmless */

    /* get the n‑th prime (1‑based indexing) */
    uint64_t pn = get_prime(n);    /* pn ≤ ~1e5 for n ≤ 10000 */
    uint64_t m  = pn + 1;          /* m = (p-1)(q-1) */

    seq_t count = 0;

    /* iterate over divisors d of m, d ≤ sqrt(m) */
    for (uint64_t d = 1; d * d <= m; ++d) {
        if (m % d == 0) {
            uint64_t p = d + 1;
            if (is_prime_seq((seq_t)p)) {
                uint64_t e = m / d;
                uint64_t q = e + 1;
                if (is_prime_seq((seq_t)q)) {
                    /* p ≤ q automatically holds because d ≤ e */
                    count++;
                }
            }
        }
    }

    return count;
}

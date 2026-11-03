/*
 * A147873  A sequence based on the mechanics of A147781: b(n) = Apply[Plus,
 *          IntegerDigits( 17982*Sum_{m=0..7} Prime(n+m) )]; a(n) = 1-(b(n)
 *          mod 2).
 * https://oeis.org/A147873
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less, uned
 * Formula:   a(n) = 1 - (b(n) mod 2) * where b(n) = digit sum of 17982 *
 *            Σ_{m=0..7} Prime(n+m)
 * Range:     u8 n <= 3, u16 none, u32 n <= 100, u64 n <= 100, u128 n <= 100,
 *            big any n
 * Validated: partial, pass, 100 terms through n = 100 (2026-11-01)
 * Helpers:   get_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/get_digits.h"

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
    /* OEIS offset is 1: a(1) is the first term. */
    if (n == 0) return (seq_t)0;      /* defensive: undefined for n<1 */

    /* Sum of 8 consecutive primes starting at the nth prime. */
    seq_t sum = 0;
    for (idx_t i = 0; i < 8; ++i)
        sum += (seq_t)get_prime(n + i);

    /* Product with the constant 17982. */
    seq_t prod = sum * (seq_t)17982;

    /* Extract decimal digits of prod and compute parity of their sum. */
    uint8_t digits[64];
    size_t len = 0;
    get_digits(prod, digits, &len);

    unsigned parity = 0;               /* parity of digit sum (mod 2) */
    for (size_t i = 0; i < len; ++i)
        parity ^= (digits[i] & 1u);

    /* a(n) = 1 if digit sum even, else 0. */
    return (parity == 0) ? (seq_t)1 : (seq_t)0;
}

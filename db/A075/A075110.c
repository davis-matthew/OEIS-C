/*
 * A075110  Concatenation of n-th prime and n in decimal notation.
 * https://oeis.org/A075110
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A000040(n) * 10^(A004216(n)+1) + n (concatenate the n‑th
 *            prime and the decimal representation of n)
 * Range:     u8 n <= 9, u16 n <= 99, u32 n <= 9999, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 56 terms through n = 9251 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

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
    /* offset is 1; a(0) is undefined – return 0 for safety */
    if (n == 0)
        return (seq_t)0;

    /* 1. obtain the n‑th prime (as a uint32_t), cast to seq_t          */
    seq_t prime = (seq_t)get_prime(n);

    /* 2. count decimal digits of n                                      */
    idx_t digits = 0;
    idx_t tmp    = n;
    do {
        ++digits;
        tmp /= 10;
    } while (tmp);

    /* 3. compute 10^digits as seq_t                                   */
    seq_t pow10 = (seq_t)1;
    idx_t i;
    for (i = 0; i < digits; ++i)
        pow10 *= (seq_t)10;

    /* 4. concatenate: prime * 10^digits + n                            */
    seq_t result = prime * pow10 + (seq_t)n;

    return result;
}

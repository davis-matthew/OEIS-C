/*
 * A140497  a(n) = 2p - number of digits of p^p, where p = n-th prime.
 * https://oeis.org/A140497
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, sign
 * Formula:   a(n) = 2 * p – floor(p * log10(p)) – 1, where p = n-th prime
 * Range:     i8 n <= 50, i16 n <= 50, i32 n <= 50, i64 n <= 50, i128 n <= 50,
 *            big any n
 * Validated: partial, pass, 50 terms through n = 50 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
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
    if (n == (idx_t)0)
        return (seq_t)0;                /* undefined for n = 0 */

    uint64_t p = (uint64_t)get_prime(n);

    /* Compute number of decimal digits of p^p:  floor(p*log10(p)) + 1 */
    long double ld = (long double)p;
    long double v  = ld * log10l(ld);     /* p * log10(p) */
    const long double eps = 1e-12L;       /* safety margin for rounding */
    unsigned long long digits = (unsigned long long)(v + eps) + 1ULL;

    /* Safe intermediate calculation in a wider signed integer. */
    int64_t tmp = (int64_t)2 * (int64_t)p;           /* 2 * p  */
    int64_t res = tmp - (int64_t)digits;           /* a(n)  */

    return (seq_t)res;
}

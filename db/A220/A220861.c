/*
 * A220861  Choose smallest m>0 such that the n-th rational prime p ramifies
 *          in the imaginary quadratic extension field K = Q(sqrt(-m)); a(n) =
 *          discriminant(K).
 * https://oeis.org/A220861
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -4 if p=2; -p if p≡3 (mod 4); -4p if p≡1 (mod 4)
 * Range:     i8 n <= 11, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Validated: standard, pass, 62 terms through n = 940 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses:      get_prime(idx_t)  – returns the n‑th prime (p₁=2) */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

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
#include <stdint.h>

seq_t a(idx_t n)
{
    uint32_t p = get_prime(n);           /* nth rational prime */
    if (p == 2)                          /* only p=2 has m=1 */
        return (seq_t)-4;

    if ((p & 3) == 3)                    /* p ≡ 3 (mod 4)  */
        return -(seq_t)p;

    /* p ≡ 1 (mod 4) */
    int64_t val = -(int64_t)4 * (int64_t)p;
    return (seq_t)val;
}

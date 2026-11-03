/*
 * A037126  Triangle T(n,k) = prime(k) for k = 1..n.
 * https://oeis.org/A037126
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = A000040(n - t*(t+1)/2) where t =
 *            floor((-1+sqrt(8*n-7))/2)
 * Range:     u8 n <= 1539, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Validated: standard, pass, 101 terms through n = 5039 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
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

/* Integer square root for a 128‑bit unsigned integer. */
static inline idx_t isqrt128(__uint128_t x)
{
    idx_t lo = 0;
    idx_t hi = (idx_t)-1;          /* maximum idx_t value */
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        __uint128_t mid2 = (__uint128_t)mid * (__uint128_t)mid;
        if (mid2 == x)
            return mid;
        if (mid2 < x)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;
}

seq_t a(idx_t n)
{
    /* Compute row number t. */
    __uint128_t val = (__uint128_t)8 * n - 7;      /* 8*n-7 fits in 128 bits */
    idx_t sq   = isqrt128(val);                    /* floor(sqrt(8*n-7))   */
    idx_t t    = (sq + 1) >> 1;                    /* (sq+1)/2             */

    /* Compute k = n - (t-1)*t/2, the column index (1‑based). */
    __uint128_t prev_prod = (__uint128_t)(t - 1) * (__uint128_t)t;
    idx_t prev_sum = (idx_t)(prev_prod >> 1);      /* divide by 2 */
    idx_t k = n - prev_sum;

    /* Return the k‑th prime. */
    return (seq_t)get_prime(k);
}

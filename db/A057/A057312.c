/*
 * A057312  Coordination sequence T1 for Zeolite Code SAS.
 * https://oeis.org/A057312
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 192, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence derived from the rational generating function
   of OEIS A057312 (Coordination sequence T1 for Zeolite Code SAS).

   Denominator:  (1 - x)^3 (1 - x + x^2) (1 + x + x^2 + x^3 + x^4)^2
   Numerator:    (1 + x)(1 + x^2)(1 + x + 2x^2 + 2x^3 + 3x^4 + 4x^5 + 3x^6 + 2x^7 + 2x^8 + x^9 + x^10)

   Let Q(x) = Σ_{k=0}^{13} q_k x^k   (q_0 = 1)
   Let P(x) = Σ_{k=0}^{13} p_k x^k

   Then for n ≥ 0

          a[n] + Σ_{k=1}^{13} q_k a[n-k] = p_n      (p_n = 0 for n > 13)

   Initial values a[0]..a[13] are obtained from the inhomogeneous part
   of the recurrence.  The function below computes a[n] for any n ≥ 0.
*/

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#ifdef __cplusplus
#include <algorithm>
#include <cstring>
#endif

/* Coefficients q_k of the denominator polynomial Q(x).  q_0 is unused. */
static const int Q[14] = {
    1,   /* q_0 (unused) */
   -2,   /* q_1 */
    2,   /* q_2 */
   -1,   /* q_3 */
    0,   /* q_4 */
   -2,   /* q_5 */
    4,   /* q_6 */
   -4,   /* q_7 */
    2,   /* q_8 */
    0,   /* q_9 */
    1,   /* q_10 */
   -2,   /* q_11 */
    2,   /* q_12 */
   -1    /* q_13 */
};

/* Coefficients p_k of the numerator polynomial P(x). */
static const int P[14] = {
    1,   /* p_0 */
    2,   /* p_1 */
    4,   /* p_2 */
    6,   /* p_3 */
    8,   /* p_4 */
   11,   /* p_5 */
   12,   /* p_6 */
   12,   /* p_7 */
   11,   /* p_8 */
    8,   /* p_9 */
    6,   /* p_10 */
    4,   /* p_11 */
    2,   /* p_12 */
    1    /* p_13 */
};

/* Allocate a new array of seq_t of given capacity.
   The array is zero-initialised. */
static seq_t *alloc_array(size_t cap)
{
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t *)calloc(cap, sizeof(seq_t));
#endif
}

/* Ensure that the cached array can store indices up to n (inclusive). */
static void ensure_cache_capacity(idx_t n, seq_t **cache, size_t *cache_cap,
                                  size_t *cache_len)
{
    size_t needed = (size_t)n + 1;
    if (needed <= *cache_cap) return;

    size_t newcap = (*cache_cap) ? (*cache_cap) * 2 : 8;
    while (newcap < needed) newcap *= 2;

    seq_t *newptr = alloc_array(newcap);
#if defined(__cplusplus)
    if (*cache && *cache_len) {
        std::copy(*cache, *cache + *cache_len, newptr);
        delete[] *cache;
    }
#else
    if (*cache && *cache_len) {
        memcpy(newptr, *cache, (*cache_len) * sizeof(seq_t));
        free(*cache);
    }
#endif
    *cache = newptr;
    *cache_cap = newcap;
    /* cache_len does not change */
}

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;   /* allocated capacity */
    static size_t cache_len  = 0;   /* number of terms stored (next free index) */

    /* Ensure the cache can hold indices up to n. */
    ensure_cache_capacity(n, &cache, &cache_cap, &cache_len);

    /* Compute missing terms up to n. */
    size_t start = cache_len;
    size_t end   = (size_t)n;
    for (size_t i = start; i <= end; ++i) {
        int64_t sum = 0;                      /* Σ q_k * a[i-k]  (k=1..min(i,13)) */
        size_t maxk = (i < 13) ? i : 13;
        for (size_t k = 1; k <= maxk; ++k) {
            sum += (int64_t)cache[i - k] * (int64_t)Q[k];
        }

        int64_t val;
        if (i <= 13)           /* inhomogeneous part */
            val = (int64_t)P[i] - sum;
        else                   /* homogeneous recurrence */
            val = -sum;

        cache[i] = (seq_t)val;
    }

    cache_len = end + 1;
    return cache[(size_t)n];
}

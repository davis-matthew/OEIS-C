/*
 * A049608  a(n)=T(n,n+2), array T as in A049600.
 * https://oeis.org/A049608
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Recurrence for A049608:
 *   (n-1)*(2*n-1)*(n+2)*a(n) -12*n*(n-1)*(n+1)*a(n-1) +
 *   (n-2)*(2*n+1)*(n+1)*a(n-2) = 0
 *   a(0) = 0, a(1) = 1, a(2) = 6
 */

#if defined(__SIZEOF_INT128__)
// Use 128‑bit arithmetic for intermediate values when seq_t is 64‑bit.
typedef unsigned __int128 uint128_t;
#  define HAVE_INT128 1
#else
#  define HAVE_INT128 0
#endif

/* Cached computed values for any access pattern */
static seq_t *cache      = NULL; /* array of terms */
static size_t cache_len  = 0;    /* elements already initialized */
static size_t cache_cap  = 0;    /* allocated capacity */

/* Return a(n) for the OEIS sequence A049608 */
seq_t a(idx_t n)
{
    /* Handle trivial small indices directly. */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    /* Ensure cache can hold indices 0..n. */
    size_t needed = (size_t)n + 1;
    if (needed > cache_cap) {
        seq_t *tmp = (seq_t*)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) return (seq_t)0;     /* out of memory – abort silently */
        cache = tmp;
        cache_cap = needed;
    }

    /* Initialize base values if not already done. */
    if (cache_len == 0) {
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        if (cache_cap < 3) {          /* make room for index 2 if needed */
            seq_t *tmp2 = (seq_t*)realloc(cache, 3 * sizeof(seq_t));
            if (!tmp2) return (seq_t)0;
            cache = tmp2;
            cache_cap = 3;
        }
        cache[2] = (seq_t)6;
        cache_len = 3;
    }

    /* Compute any missing terms up to n. */
    for (idx_t i = (idx_t)cache_len; i <= n; ++i) {
        seq_t i_seq   = (seq_t)i;
        seq_t prev1   = cache[i-1];
        seq_t prev2   = cache[i-2];

#if HAVE_INT128
        uint128_t mul1 = (uint128_t)12;
        mul1 *= (uint128_t)i_seq;
        mul1 *= (uint128_t)(i_seq - 1);
        mul1 *= (uint128_t)(i_seq + 1);
        uint128_t term1 = (uint128_t)prev1 * mul1;

        uint128_t mul2 = (uint128_t)(i_seq - 2);
        mul2 *= (uint128_t)(2 * i_seq + 1);
        mul2 *= (uint128_t)(i_seq + 1);
        uint128_t term2 = (uint128_t)prev2 * mul2;

        uint128_t num = term1 - term2;

        uint128_t denom = (uint128_t)(i_seq - 1);
        denom *= (uint128_t)(2 * i_seq - 1);
        denom *= (uint128_t)(i_seq + 2);

        cache[i] = (seq_t)(num / denom);
#else
        seq_t term1 = prev1;
        term1 *= (seq_t)12;
        term1 *= i_seq;
        term1 *= (i_seq - 1);
        term1 *= (i_seq + 1);

        seq_t term2 = prev2;
        term2 *= (i_seq - 2);
        term2 *= (2 * i_seq + 1);
        term2 *= (i_seq + 1);

        seq_t num = term1 - term2;

        seq_t denom = (i_seq - 1);
        denom *= (2 * i_seq - 1);
        denom *= (i_seq + 2);

        cache[i] = num / denom;
#endif
    }

    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}

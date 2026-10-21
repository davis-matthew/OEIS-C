/*
 * A220898  Number of primitive maps on n edges.
 * https://oeis.org/A220898
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 17, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   alloc_seq, free_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A220898: Number of primitive maps on n edges.
   Recurrence (n ≥ 3):
     2*(n+1)*(2*n+1)*a(n) + (-15*n^2 + 19*n - 8)*a(n-1)
     - 2*(21*n-22)*(n-2)*a(n-2) - 23*(n-2)*(n-3)*a(n-3) = 0
   Initial values:  a(0)=2, a(1)=1, a(2)=1.
   The implementation uses a cache so that calls may appear in any order. */

#include <stddef.h>
#include <stdint.h>
#include "../utils/alloc_seq.h"
#include "../utils/free_seq.h"

static seq_t *cache = NULL;     /* cached terms */
static size_t cache_len = 0;    /* number of filled entries      */
static size_t cache_cap = 0;    /* allocated capacity            */

/* enlarge cache so that it can hold at least 'needed' entries */
static void extend_cache(size_t needed)
{
    if (cache_cap >= needed) return;
    size_t new_cap = cache_cap ? cache_cap : 4;
    while (new_cap < needed) new_cap <<= 1;   /* double until big enough */

    seq_t *new_cache = (seq_t *)alloc_seq(new_cap);
    for (size_t i = 0; i < cache_len; ++i) new_cache[i] = cache[i];
    if (cache) free_seq(cache);
    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* initialise cache on first call */
    if (cache_len == 0) {
        extend_cache(3);          /* indices 0,1,2 */
        cache[0] = (seq_t)2;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache_len = 3;
    }

    /* ensure capacity up to n */
    if ((size_t)n + 1 > cache_cap) extend_cache((size_t)n + 1);

    /* fill missing values using the recurrence */
    if (cache_len <= (size_t)n) {
        for (size_t i = cache_len; i <= (size_t)n; ++i) {
            /* local index as integer type */
            idx_t ii = (idx_t)i;

#if defined(__cplusplus)
            /* C++ branch: use seq_t arithmetic directly */
            seq_t s_i = (seq_t)ii;

            seq_t coeff1 = seq_t(15) * s_i * s_i
                           - seq_t(19) * s_i
                           + seq_t(8);

            seq_t coeff2 = seq_t(2) *
                           (seq_t(21) * s_i - seq_t(22)) *
                           (s_i - seq_t(2));

            seq_t coeff3 = seq_t(23) *
                           (s_i - seq_t(2)) *
                           (s_i - seq_t(3));

            seq_t term1  = coeff1 * cache[i-1];
            seq_t term2  = coeff2 * cache[i-2];
            seq_t term3  = coeff3 * cache[i-3];

            seq_t numerator   = term1 + term2 + term3;
            seq_t denominator = seq_t(2) * (s_i + seq_t(1)) *
                               (seq_t(2) * s_i + seq_t(1));

            cache[i] = numerator / denominator;
#else
            /* C branch: use __int128 intermediate to avoid overflow */
            __int128 i_val    = (__int128)ii;
            __int128 coeff1   = (__int128)15 * i_val * i_val
                               - (__int128)19 * i_val
                               + (__int128)8;

            __int128 coeff2   = (__int128)2 *
                               ( (__int128)21 * i_val - (__int128)22) *
                               (i_val - (__int128)2);

            __int128 coeff3   = (__int128)23 *
                               (i_val - (__int128)2) *
                               (i_val - (__int128)3);

            __int128 term1 = coeff1 * (__int128)cache[i-1];
            __int128 term2 = coeff2 * (__int128)cache[i-2];
            __int128 term3 = coeff3 * (__int128)cache[i-3];

            __int128 numer = term1 + term2 + term3;
            uint64_t denom = 2U * (ii + 1U) * (2U * ii + 1U);

            cache[i] = (seq_t)(numer / denom);
#endif
        }
        cache_len = (size_t)n + 1;
    }

    return cache[n];
}

/*
 * A151548  When A160552 is regarded as a triangle with rows of lengths 1, 1,
 *          2, 4, 8, 16, ..., this is what the rows converge to.
 * https://oeis.org/A151548
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(2^k-1) = 2^(k+1)-1 for k >= 0 a(2^k) = 5 for k >= 1 otherwise
 *            a(2^i+j) = 2*a(j)+a(j+1) for i>=2, 1<=j<=2^i-2 Initial values:
 *            a(0)=1, a(1)=3
 * Range:     u8 n <= 59, u16 n <= 2042, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_log2_uint64, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/floor_log2_uint64.h"   /* unsigned floor_log2_uint64(idx_t v); */
#include "../utils/pow2.h"                /* seq_t pow2(idx_t exp); */

static seq_t *cache = NULL;     /* memoisation array */
static idx_t cache_len = 0;     /* number of values stored */

/* Return a(n) for the OEIS sequence A151548 */
seq_t a(idx_t n)
{
    /* Ensure we have enough space in the cache */
    if (cache_len <= n) {
        idx_t new_len = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache) { /* allocation failed: abort */
            abort();
        }
        cache = new_cache;
        /* Compute new entries sequentially */
        for (idx_t i = cache_len; i < new_len; ++i) {
            if (i == 0) {
                cache[i] = (seq_t)1;
            } else if (i == 1) {
                cache[i] = (seq_t)3;
            } else {
                /* Check for the special form 2^k-1 (n+1 is power of two) */
                if ( ((i + 1) & i) == 0 ) {
                    /* i+1 is a power of two */
                    unsigned k = floor_log2_uint64(i + 1); /* 2^k = i+1 */
                    seq_t v = pow2(k + 1);                /* 2^(k+1) */
                    cache[i] = v - 1;                     /* 2^(k+1) - 1 */
                }
                /* Check whether i itself is a power of two (>=2) */
                else if ( (i & (i - 1)) == 0 && i >= 2 ) {
                    cache[i] = (seq_t)5;
                }
                /* General recurrence */
                else {
                    /* Find largest power of two <= i */
                    unsigned p_log = floor_log2_uint64(i);
                    idx_t p = 1ULL << p_log;   /* 2^p_log */
                    idx_t j = i - p;           /* 0 < j <= p-2 (i is not a power of two or 2^k-1) */
                    cache[i] = (seq_t)2 * cache[j] + cache[j + 1];
                }
            }
        }
        cache_len = new_len;
    }
    return cache[n];
}

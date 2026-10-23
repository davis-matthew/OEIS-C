/*
 * A048563  a(n+1) is next smallest nontrivial cube beginning with a(n),
 *          initial cube is 1.
 * https://oeis.org/A048563
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Helpers:   ensure_cache, floor_cube_root, mul_small, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A048563:
   a(n+1) is the smallest cube > a(n) whose decimal representation starts
   with the digits of a(n) and is not a trivial repetition of a(n) with
   trailing zeros.  a(0) = 1. */

#include <stdbool.h>
#include "../utils/ensure_cache.h"
#include "../utils/floor_cube_root.h"
#include "../utils/seq_pow.h"
#include "../utils/mul_small.h"
#include "../utils/seq_mul.h"

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_size = 0;

seq_t a(idx_t n)
{
    /* make sure we have room for indices up to n */
    ensure_cache(n, &cache, &cache_cap);

    /* initialize the first term */
    if (cache_size == 0) {
        cache[0] = (seq_t)1;
        cache_size = 1;
    }

    if (n < cache_size)
        return cache[n];

    /* compute missing terms up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t prefix = cache[i-1];
        seq_t pow10  = (seq_t)1;  /* 10^d, starts at d = 0 */

        seq_t next_cube;
        for (;;) {
            /* bounds for cubes that start with 'prefix' */
            seq_t lower = seq_mul(prefix, pow10);                      /* prefix*10^d   */
            seq_t upper = seq_mul(prefix + (seq_t)1, pow10);          /* (prefix+1)*10^d */
            upper -= (seq_t)1;                                        /* minus one      */

            /* smallest integer k with k^3 >= lower */
            seq_t k = floor_cube_root(lower);
            seq_t cube = seq_pow(k, (idx_t)3);
            if (cube < lower) {
                ++k;
                cube = seq_pow(k, (idx_t)3);
            }

            /* ensure cube is strictly greater than prefix*10^d */
            if (cube <= lower) {
                ++k;
                cube = seq_pow(k, (idx_t)3);
            }

            /* if cube fits the interval and is > lower, we found it */
            if (cube <= upper) {
                next_cube = cube;
                break;
            }

            /* otherwise increase the number of trailing zeros searched */
            pow10 = mul_small(pow10, 10);
        }

        cache[i] = next_cube;
    }

    cache_size = n + 1;
    return cache[n];
}

/*
 * A183053  Sums of knight's moves over the square |i|+|j|<=n on infinite
 *          chessboard.
 * https://oeis.org/A183053
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 35, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  a(0)=0, a(1)=12, a(2)=28, a(3)=48, a(4)=88, a(5)=148,
 * a(6)=220, a(7)=312, a(8)=440, a(9)=588, a(10)=772, a(11)=1000,
 * a(12)=1248, a(13)=1548.
 *
 * For n ≥ 14:
 * a(n) =  2*a(n-1) - 2*a(n-2) + 4*a(n-3)
 *        - 5*a(n-4) + 4*a(n-5) - 5*a(n-6)
 *        + 4*a(n-7) - 2*a(n-8) + 2*a(n-9) - a(n-10).
 *
 * The recurrence is derived from the given generating function.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_size = 0;

seq_t a(idx_t n)
{
    /* Initialise the cache with the first 14 values if it hasn't been done. */
    if (cache_size == 0U)
    {
        cache_size = (n < 14U ? 14U : n + 1U);   /* at least 14 entries */
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache)
            return 0;   /* out of memory; caller may ignore result */

        static const seq_t init_vals[14] = {
            0U,    12U,   28U,   48U,   88U,   148U,   220U,   312U,
            440U,  588U,  772U,  1000U, 1248U, 1548U
        };

        for (size_t i = 0U; i < 14U; ++i)
            cache[i] = init_vals[i];
    }

    /* Extend the cache if the requested index is beyond the current size. */
    if ((size_t)n + 1U > cache_size)
    {
        size_t new_size = (size_t)n + 1U;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp)
            return 0;   /* out of memory */

        /* Compute missing values using the recurrence. */
        for (idx_t idx = (idx_t)(cache_size); idx <= n; ++idx)
        {
            seq_t val =
                2U * tmp[idx - 1] - 2U * tmp[idx - 2] +
                4U * tmp[idx - 3] - 5U * tmp[idx - 4] +
                4U * tmp[idx - 5] - 5U * tmp[idx - 6] +
                4U * tmp[idx - 7] - 2U * tmp[idx - 8] +
                2U * tmp[idx - 9] - tmp[idx - 10];

            tmp[idx] = val;
        }

        cache = tmp;
        cache_size = new_size;
    }

    return cache[(size_t)n];
}

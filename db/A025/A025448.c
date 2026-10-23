/*
 * A025448  Number of partitions of n into 4 nonnegative cubes.
 * https://oeis.org/A025448
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into 4 nonnegative cubes
 * Range:     u8 n <= 107, u16 n <= 107, u32 n <= 107, u64 n <= 107,
 *            u128 n <= 107, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Integer cube‑root floor (returns the greatest r with r³ ≤ n). */
static inline idx_t cube_root_floor(idx_t n)
{
    idx_t r = 0;
    while (1)
    {
        idx_t t = r + 1;
        if (t * t * t > n) break;
        r = t;
    }
    return r;
}

/* Main sequence function.  It builds and caches all counts up to the
   largest n requested so far.  The cache is reallocated when a
   larger n is seen. */
seq_t a(idx_t n)
{
    /* Static cache holding counts for 0 … cache_len‑1 */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;  /* elements stored */

    /* Return stored value if it is already known */
    if (n < cache_len)
        return cache[n];

    /* Allocate a new cache large enough for 0 … n */
    idx_t new_len = n + 1;

    /* Allocate array of seq_t initialized to zero */
#ifdef __cplusplus
    seq_t *new_cache = new seq_t[new_len];
#else
    seq_t *new_cache = (seq_t *)calloc((size_t)new_len, sizeof(seq_t));
#endif
    if (!new_cache)  /* allocation failed */
        return (seq_t)0;

    /* Compute all partitions of numbers ≤ n into 4 non‑negative cubes.
       Use non‑decreasing order a ≤ b ≤ c ≤ d to avoid counting
       permutations separately. */
    idx_t max_cub = cube_root_floor(n);
    for (idx_t a = 0; a <= max_cub; ++a)
    {
        seq_t ca = (seq_t)a * a * a;
        for (idx_t b = a; b <= max_cub; ++b)
        {
            seq_t cb = ca + (seq_t)b * b * b;
            for (idx_t c = b; c <= max_cub; ++c)
            {
                seq_t cc = cb + (seq_t)c * c * c;
                for (idx_t d = c; d <= max_cub; ++d)
                {
                    seq_t sum = cc + (seq_t)d * d * d;
                    if (sum < (seq_t)new_len)              /* sum fits in index range */
                    {
                        idx_t idx = (idx_t)sum;            /* safe because sum < new_len ≤ 2^64-1 */
                        new_cache[idx] += (seq_t)1;
                    }
                }
            }
        }
    }

    /* Replace the old cache */
    if (cache)
    {
#ifdef __cplusplus
        delete [] cache;
#else
        free(cache);
#endif
    }
    cache = new_cache;
    cache_len = new_len;

    return cache[n];
}

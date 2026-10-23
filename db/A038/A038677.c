/*
 * A038677  Cubes ending in a (different) positive cube.
 * https://oeis.org/A038677
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 6, u32 n <= 283, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   floor_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A038677: cubes whose decimal representation ends with a different
   positive cube.  The code enumerates m = 1, 2, …, computes k = m³,
   and checks all decimal suffixes of k to see whether one is a cube
   r³ with r > 0 and r ≠ m.  The found cubes are cached so that any
   requested term a(n) (offset 1) can be returned in O(1) time after
   the required number of terms has been discovered. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/floor_cube_root.h"

/* Count decimal digits of x.  Works for any non‑negative seq_t. */
static int digits(seq_t x)
{
    int d = 0;
    do
    {
        x /= 10;
        ++d;
    } while (x > 0);
    return d;
}

/* Return true if k has a decimal suffix that is a cube of a
   different integer than base. */
static bool has_suffix_cube(seq_t k, seq_t base)
{
    int L = digits(k);
    seq_t pow10k        = 10;   /* 10^klen for current suffix length */
    seq_t pow10k_minus = 1;     /* 10^(klen-1) for non‑zero check */

    for (int klen = 1; klen <= L; ++klen)
    {
        seq_t suffix = k % pow10k;
        if (suffix >= pow10k_minus)            /* no leading zero */
        {
            seq_t r = floor_cube_root(suffix);
            if (r != 0 && r != base)
            {
                seq_t rcub = r;
                rcub *= r;
                rcub *= r;
                if (rcub == suffix)
                    return true;
            }
        }
        pow10k        *= 10;
        pow10k_minus *= 10;
    }
    return false;
}

/* Cache of already found terms */
static seq_t *cache   = NULL;
static size_t cache_size = 0, cache_cap = 0;
static idx_t  last_m   = 0;

/* Append value to cache, growing if necessary. */
static void cache_append(seq_t v)
{
    if (cache_size == cache_cap)
    {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *new_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (cache)
            memcpy(new_arr, cache, cache_size * sizeof(seq_t));
        free(cache);
        cache = new_arr;
        cache_cap = new_cap;
    }
    cache[cache_size++] = v;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)               /* offset is 1; for n<1 return 0 */
        return 0;

    while (cache_size < n)
    {
        ++last_m;
        seq_t m     = (seq_t)last_m;
        seq_t cube  = m;
        cube *= m;
        cube *= m;
        if (has_suffix_cube(cube, m))
            cache_append(cube);
    }
    return cache[n - 1];
}

/*
 * A123063  Theta series of lattice with Gram matrix [4,1;1,8].
 * https://oeis.org/A123063
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer solutions to n = 2*x^2 + x*y + 4*y^2
 *            (x,y ∈ ℤ) The implementation precomputes all values up to the
 *            required index by brute‑forcing the bounded set of (x,y) pairs
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* static cache for the sequence values 0..cache_len-1 */
    static seq_t *cache = NULL;
    static idx_t   cache_len = 0;

    /* If we have not computed enough terms yet, extend the cache */
    if (!cache || n + 1 > cache_len) {
        /* Allocate a fresh cache sized to hold all values up to n */
        seq_t *new_cache = (seq_t *)calloc(n + 1, sizeof(seq_t));
        if (!new_cache) abort();            /* out of memory */

        /* Convert the index n to seq_t for arithmetic */
        idx_t N = n;

        /* Bounds for |y| and |x| that guarantee 2*x^2 ≤ N and 4*y^2 ≤ N.
           Adding +1 guarantees that the extreme pairs are included. */
        int Ymax = (int)floor_sqrt(N / 4) + 1;
        int Xmax = (int)floor_sqrt(N / 2) + 1;

        /* Enumerate all pairs (x,y) with the above bounds. */
        for (int y = -Ymax; y <= Ymax; ++y) {
            /* Pre‑compute 4*y^2 to avoid recomputing inside the inner loop. */
            long long yy = (long long)y;
            long long four_y2 = 4LL * yy * yy;

            for (int x = -Xmax; x <= Xmax; ++x) {
                long long xx = (long long)x;
                long long val = 2LL * xx * xx + xx * yy + four_y2;
                if (val < 0 || (idx_t)val > N) continue;   /* safety check */
                new_cache[(idx_t)val] += 1;
            }
        }

        /* Replace old cache */
        free(cache);
        cache     = new_cache;
        cache_len = n + 1;
    }

    return cache[n];
}

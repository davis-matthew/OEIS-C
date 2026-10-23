/*
 * A049747  Array T read by diagonals; T(i,j)=number of directions from (0,0)
 *          to lattice points in first-quadrant portion of disk with radius
 *          (0,0)-to-(i,j).
 * https://oeis.org/A049747
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   antidiagonal_coords, floor_sqrt, gcd, grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A049747: for each n, convert n to (i,j) on the
   antidiagonal, form R^2 = i^2 + j^2, and count primitive lattice
   directions (x,y) in the first–quadrant disk of radius R, i.e.
   gcd(x,y)=1, x≥0,y≥0, x^2+y^2 ≤ R^2.  The count includes the two
   axis directions (1,0) and (0,1) when R≥1. */
#include <stdint.h>
#include "../utils/grow_seq.h"
#include "../utils/antidiagonal_coords.h"
#include "../utils/floor_sqrt.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* used indices */
    static size_t cache_cap = 0;   /* allocated capacity */

    /* grow cache if needed */
    if ((size_t)n >= cache_len) {
        grow_seq(&cache, &cache_cap, (size_t)n + 1);

        for (size_t k = cache_len; k <= (size_t)n; ++k) {
            idx_t i, j;
            antidiagonal_coords(k, &i, &j);

            seq_t r2 = (seq_t)i * (seq_t)i + (seq_t)j * (seq_t)j;
            seq_t cnt = 0;

            if (r2 == 0) {            /* (0,0) – no directions */
                cnt = 0;
            } else {
                /* axes directions */
                cnt += 2;

                idx_t max_y = (idx_t)floor_sqrt(r2);
                for (idx_t y = 1; y <= max_y; ++y) {
                    seq_t y2 = (seq_t)y * (seq_t)y;
                    seq_t rem = r2 - y2;
                    idx_t max_x = (idx_t)floor_sqrt(rem);
                    for (idx_t x = 1; x <= max_x; ++x) {
                        if (gcd((seq_t)x, (seq_t)y) == 1)
                            ++cnt;
                    }
                }
            }
            cache[k] = cnt;
        }
        cache_len = (size_t)n + 1;
    }
    return cache[n];
}

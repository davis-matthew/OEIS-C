/*
 * A224215  Number of nonnegative solutions to x^3 + y^3 + z^3 <= n^3.
 * https://oeis.org/A224215
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of nonnegative integer solutions to x^3 + y^3 +
 *            z^3 <= n^3
 * Range:     u8 n <= 5, u16 n <= 39, u32 n <= 1317, u64 n <= 1500,
 *            u128 n <= 1500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Precompute cubes up to n^3 */
    idx_t np1 = n + 1;
    seq_t *cubes = (seq_t*)malloc(np1 * sizeof(seq_t));
    if (!cubes) return (seq_t)0; /* allocation failure, return 0 */

    seq_t n3 = (seq_t)n * n * n;
    for (idx_t i = 0; i < np1; ++i) {
        seq_t ii = (seq_t)i;
        cubes[i] = ii * ii * ii;
    }

    seq_t ans = 0;
    for (idx_t x = 0; x < np1; ++x) {
        seq_t cx = cubes[x];
        int z = (int)n; /* start with z = n */
        for (idx_t y = 0; y < np1; ++y) {
            seq_t cy = cubes[y];
            seq_t sum_xy = cx + cy;
            /* decrease z until sum_xy + z^3 <= n3 */
            while (z >= 0 && sum_xy + cubes[z] > n3)
                z--;
            if (z < 0) break; /* no more valid z for larger y */
            ans += (seq_t)(z + 1); /* z ranges 0..z inclusive */
        }
    }

    free(cubes);
    return ans;
}

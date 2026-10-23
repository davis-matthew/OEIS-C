/*
 * A025464  Number of partitions of n into 2 distinct nonnegative cubes.
 * https://oeis.org/A025464
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Number of partitions of n into two distinct non‑negative cubes
 * Range:     u8 n <= 63, u16 n <= 107, u32 n <= 107, u64 n <= 107,
 *            u128 n <= 107, big any n
 * Helpers:   floor_cube_root, int_cube_root, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/int_cube_root.h"
#include "../utils/floor_cube_root.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    seq_t count = 0;
    seq_t sn = n;
    idx_t max_a = int_cube_root(n);
    for (idx_t a = 0; a <= max_a; ++a)
    {
        seq_t aa = a;
        seq_t acube = aa * aa * aa;
        if (acube >= sn)
            break;
        seq_t rem = sn - acube;
        seq_t b = floor_cube_root(rem);
        if (b <= aa)
            continue;
        seq_t bcube = seq_pow(b, 3);
        if (bcube == rem)
            count++;
    }
    return count;
}

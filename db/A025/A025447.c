/*
 * A025447  Number of partitions of n into 3 nonnegative cubes.
 * https://oeis.org/A025447
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9917, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   int_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count the number of partitions of n into three nonnegative cubes. */
#include "../utils/int_cube_root.h"

seq_t a(idx_t n)
{
    seq_t count = 0;
    idx_t maxA = int_cube_root(n);               /* floor(cuberoot(n)) */

    for (idx_t c = 0; c <= maxA; ++c) {
        seq_t cubeC = (seq_t)c * (seq_t)c * (seq_t)c;
        idx_t rem1 = n - (idx_t)cubeC;           /* remaining after c^3 */

        idx_t maxB = int_cube_root(rem1);
        for (idx_t b = c; b <= maxB; ++b) {
            seq_t cubeB = (seq_t)b * (seq_t)b * (seq_t)b;
            idx_t rem2 = rem1 - (idx_t)cubeB;    /* remaining after b^3 */

            idx_t a_root = int_cube_root(rem2);
            if (a_root < b) continue;           /* enforce a >= b */

            seq_t cubeA = (seq_t)a_root * (seq_t)a_root * (seq_t)a_root;
            if (cubeA == rem2)                  /* found a valid partition */
                ++count;
        }
    }
    return count;
}

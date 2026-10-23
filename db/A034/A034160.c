/*
 * A034160  Number of partitions of n into 3 distinct parts from [ 1,17 ].
 * https://oeis.org/A034160
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{ (x,y,z) : 1 <= x < y < z <= 17, x + y + z = n }|
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t table[49] = {0};
    static bool initialized = false;

    if (!initialized) {
        for (int x = 1; x <= 17; ++x) {
            for (int y = x + 1; y <= 17; ++y) {
                for (int z = y + 1; z <= 17; ++z) {
                    idx_t sum = (idx_t)x + (idx_t)y + (idx_t)z;
                    if (sum < 49) {
                        table[sum] += 1;
                    }
                }
            }
        }
        initialized = true;
    }

    return (n < 49) ? table[n] : 0;
}

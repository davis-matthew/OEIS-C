/*
 * A034151  Number of partitions of n into 3 distinct parts from [ 1,8 ].
 * https://oeis.org/A034151
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Count of partitions of n into 3 distinct parts from the set
 *            {1,2,3,4,5,6,7,8}
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* That is, the number of integer triples 1 <= i < j < k <= 8 such that i + j + k == n. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* The maximum possible sum is 6 + 7 + 8 = 21. */
    if (n > 21)
        return 0;

    seq_t count = 0;
    for (int i = 1; i <= 8; ++i) {
        for (int j = i + 1; j <= 8; ++j) {
            for (int k = j + 1; k <= 8; ++k) {
                seq_t sum = (seq_t)i + j + k;
                if (sum == n)
                    ++count;
            }
        }
    }
    return count;
}

/*
 * A034153  Number of partitions of n into 3 distinct parts from [ 1,10 ].
 * https://oeis.org/A034153
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{(a,b,c) | 1 <= a < b < c <= 10, a + b + c = n}
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Counting distinct 3-part partitions with parts in the range 1..10. */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Static table for all possible sums of distinct triples from 1..10. */
    static seq_t table[28];
    static bool initialized = false;

    if (!initialized) {
        /* Compute once. 28 entries cover sums 0..27 (max 10+9+8). */
        for (idx_t a = 1; a <= 10; ++a) {
            for (idx_t b = a + 1; b <= 10; ++b) {
                for (idx_t c = b + 1; c <= 10; ++c) {
                    idx_t s = a + b + c;
                    /* s is guaranteed < 28 */
                    table[s] = (seq_t)(table[s] + (seq_t)1);
                }
            }
        }
        initialized = true;
    }

    if (n < 28) {
        return table[n];
    } else {
        return 0;
    }
}

/*
 * A034159  Number of partitions of n into 3 distinct parts from [ 1,16 ].
 * https://oeis.org/A034159
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of triples (i,j,k) with 1 ≤ i < j < k ≤ 16 and i +
 *            j + k = n
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t table[46] = {0};   /* indices 0..45 */
    static int built = 0;

    if (!built) {
        for (idx_t i = 1; i <= 16; ++i)
            for (idx_t j = i + 1; j <= 16; ++j)
                for (idx_t k = j + 1; k <= 16; ++k) {
                    idx_t sum = i + j + k;
                    if (sum <= 45)
                        table[(size_t)sum] += (seq_t)1;
                }
        built = 1;
    }

    if (n <= 45)
        return table[(size_t)n];
    else
        return (seq_t)0;
}

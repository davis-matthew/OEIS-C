/*
 * A034154  Number of partitions of n into 3 distinct parts from [ 1,11 ].
 * https://oeis.org/A034154
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Number of partitions of n into 3 distinct parts from [1,11]
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t counts[31];
    static bool initialized = false;
    if (!initialized) {
        /* initialise all counts to 0 */
        for (idx_t i = 0; i < 31; ++i)
            counts[i] = (seq_t)0;
        /* enumerate all 3-element subsets of {1..11} */
        for (idx_t i = 1; i <= 9; ++i) {
            for (idx_t j = i + 1; j <= 10; ++j) {
                for (idx_t k = j + 1; k <= 11; ++k) {
                    idx_t s = i + j + k;               /* 6 .. 30 */
                    counts[s] += (seq_t)1;
                }
            }
        }
        initialized = true;
    }
    if (n < 31)
        return counts[n];
    return (seq_t)0;
}

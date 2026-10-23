/*
 * A034156  Number of partitions of n into 3 distinct parts from [ 1,13 ].
 * https://oeis.org/A034156
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into 3 distinct parts from
 *            [1,13]
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

/* Count partitions of n into 3 distinct parts between 1 and 13 inclusive.
   We pre-compute all possible sums (at most 36).  For n > 36 the answer is 0. */
seq_t a(idx_t n)
{
    /* static table of counts for sums 0..36 */
    static seq_t table[37] = {0};
    static int initialized = 0;

    if (!initialized) {
        for (idx_t i = 1; i <= 13; ++i) {
            for (idx_t j = i + 1; j <= 13; ++j) {
                for (idx_t k = j + 1; k <= 13; ++k) {
                    idx_t sum = i + j + k;                 /* sum ≤ 36 */
                    table[(size_t)sum] += (seq_t)1;        /* increment count */
                }
            }
        }
        initialized = 1;
    }

    if (n < 37) {
        return table[(size_t)n];
    } else {
        return (seq_t)0;
    }
}

/*
 * A034092  Number of partitions of n into 3 distinct parts from [ 1,39 ].
 * https://oeis.org/A034092
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, fini, full
 * Formula:   Count of partitions of n into 3 distinct parts from [1,39].
 *            Implementation precomputes all triples
 * Range:     u8 n <= 114, u16 n <= 114, u32 n <= 114, u64 n <= 114,
 *            u128 n <= 114, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Static cache for all possible sums (0..114). */
    static seq_t sum_counts[115] = {0};
    static bool initialized = false;

    if (!initialized) {
        for (idx_t i = 1; i <= 37; ++i) {
            for (idx_t j = i + 1; j <= 38; ++j) {
                for (idx_t k = j + 1; k <= 39; ++k) {
                    idx_t s = i + j + k;               /* 3 <= s <= 114 */
                    sum_counts[(size_t)s] += 1;
                }
            }
        }
        initialized = true;
    }

    if (n <= 114)
        return sum_counts[(size_t)n];
    else
        return (seq_t)0;
}

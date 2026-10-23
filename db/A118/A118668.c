/*
 * A118668  Number of distinct digits needed to write the n-th triangular
 *          number in decimal representation.
 * https://oeis.org/A118668
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of distinct decimal digits in the n-th triangular
 *            number n*(n+1)/2
 * Range:     u8 n <= 22, u16 n <= 426, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/digit_counts.h"

seq_t a(idx_t n)
{
    /* Compute the n-th triangular number T_n = n*(n+1)/2 */
    seq_t tn = (seq_t)n * ((seq_t)n + 1) / 2;

    /* Count occurrences of each decimal digit */
    unsigned char cnt[10] = {0};
    digit_counts(tn, cnt);

    /* Count how many distinct digits appear */
    unsigned int distinct = 0;
    for (idx_t d = 0; d < 10; ++d)
        if (cnt[d] != 0)
            ++distinct;

    return (seq_t)distinct;
}

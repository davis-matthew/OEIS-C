/*
 * A025041  Number of partitions of { 1, 2, ..., 9n } into sets of size 9.
 * https://oeis.org/A025041
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*n)!/(n!*(9!)^n)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 4, u128 n <= 6,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n) {
    if (n == 0) {
        return (seq_t)1;
    }
    seq_t result = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) {
        /* binomial(9*i, 9) = product of 9 consecutive numbers divided by 9! */
        seq_t comb = binomial(9 * i, 9);
        result = result * comb;
        result = result / (seq_t)i;   /* divide by n each step */
    }
    return result;
}

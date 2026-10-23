/*
 * A165984  Number of ways to put n indistinguishable balls into n^3
 *          distinguishable boxes.
 * https://oeis.org/A165984
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(n^3 + n - 1, n)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 13,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }
    idx_t total = n * n * n + n - 1;
    return binom(total, n);
}

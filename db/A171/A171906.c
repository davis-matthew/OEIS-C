/*
 * A171906  a(n) = number of 5 X 5 permutation matrices with trace n.
 * https://oeis.org/A171906
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(5, n) * D(5 - n) where D(k) is the number of
 *            derangements of k elements
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n > 5)
        return (seq_t)0;
    /* Derangements D(k) for k = 0..5 */
    static const seq_t der[6] = {1, 0, 1, 2, 9, 44};
    seq_t comb = binom(5, n);
    return comb * der[5 - n];
}

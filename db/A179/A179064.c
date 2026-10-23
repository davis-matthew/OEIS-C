/*
 * A179064  Number of non-attacking placements of 9 rooks on an n X n board.
 * https://oeis.org/A179064
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 9!*binomial(n,9)^2
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 11, u64 n <= 28, u128 n <= 200,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* For n < 9 the binomial coefficient is zero */
    if (n < 9)
        return 0;

    seq_t bin = binom(n, 9);     /* C(n,9) */
    seq_t res = bin * bin;       /* square the binomial */
    res *= (seq_t)362880;        /* multiply by 9! */

    return res;
}

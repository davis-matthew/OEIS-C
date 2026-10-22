/*
 * A005720  Quadrinomial coefficients.
 * https://oeis.org/A005720
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = binomial(n,2) + 7*binomial(n,3) + 10*binomial(n,4) +
 *            5*binomial(n,5) + binomial(n,6)
 * Range:     u8 n <= 5, u16 n <= 16, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    seq_t r = binom(n, 2);
    r += (seq_t)7 * binom(n, 3);
    r += (seq_t)10 * binom(n, 4);
    r += (seq_t)5 * binom(n, 5);
    r += binom(n, 6);
    return r;
}

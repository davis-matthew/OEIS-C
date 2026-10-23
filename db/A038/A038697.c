/*
 * A038697  Convolution of A000917 with A000984 (central binomial
 *          coefficients).
 * https://oeis.org/A038697
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*4^(n+1)+binomial(2*n+3, n+1)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 13, u64 n <= 28, u128 n <= 60,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t pow4  = big_pow((seq_t)4, (seq_t)(n + 1));
    seq_t term1 = (seq_t)n * pow4;
    seq_t term2 = binomial(2 * n + 3, n + 1);
    return term1 + term2;
}

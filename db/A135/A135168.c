/*
 * A135168  a(n) = 7^n + 5^n + 3^n + 2^n.
 * https://oeis.org/A135168
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 7^n + 5^n + 3^n + 2^n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

/* seq_t and idx_t are defined elsewhere by the build system. */
seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;           /* exponent as seq_t */
    seq_t pow7 = big_pow((seq_t)7, n_seq);
    seq_t pow5 = big_pow((seq_t)5, n_seq);
    seq_t pow3 = big_pow((seq_t)3, n_seq);
    seq_t pow2 = big_pow((seq_t)2, n_seq);

    return pow7 + pow5 + pow3 + pow2;
}

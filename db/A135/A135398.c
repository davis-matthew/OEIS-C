/*
 * A135398  a(n) = 3^n * 4^(n^2).
 * https://oeis.org/A135398
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^n * 4^(n^2)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 7,
 *            big any n
 * Helpers:   big_pow, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* 3^n */
    seq_t pow3 = big_pow((seq_t)3, (seq_t)n);

    /* 4^(n^2) */
    idx_t e = n * n;
    seq_t pow4 = big_pow((seq_t)4, (seq_t)e);

    /* multiply both parts */
    return seq_mul(pow3, pow4);
}

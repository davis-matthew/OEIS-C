/*
 * A081891  a(n) = 10^n - 9^n - 8^n - 7^n + 3*6^n.
 * https://oeis.org/A081891
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 10^n - 9^n - 8^n - 7^n + 3*6^n
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t pow10 = big_pow((seq_t)10, (seq_t)n);
    seq_t pow9  = big_pow((seq_t)9,  (seq_t)n);
    seq_t pow8  = big_pow((seq_t)8,  (seq_t)n);
    seq_t pow7  = big_pow((seq_t)7,  (seq_t)n);
    seq_t pow6  = big_pow((seq_t)6,  (seq_t)n);

    seq_t res = pow10 + (seq_t)3 * pow6;  /* 10^n + 3*6^n */
    res -= pow9;                         /* - 9^n */
    res -= pow8;                         /* - 8^n */
    res -= pow7;                         /* - 7^n */

    return res;
}

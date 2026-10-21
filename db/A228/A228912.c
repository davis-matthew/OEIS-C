/*
 * A228912  a(n) = 10^n - 9*9^n + 36*8^n - 84*7^n + 126*6^n - 126*5^n + 84*4^n
 *          - 36*3^n + 9*2^n - 1.
 * https://oeis.org/A228912
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10^n - 9*9^n + 36*8^n - 84*7^n + 126*6^n * - 126*5^n +
 *            84*4^n - 36*3^n + 9*2^n - 1
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 11, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   big_pow, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    seq_t pos = big_pow((seq_t)10, (seq_t)n);
    seq_t t;

    t = big_pow((seq_t)8, (seq_t)n);
    pos += mul_small(t, 36);

    t = big_pow((seq_t)6, (seq_t)n);
    pos += mul_small(t, 126);

    t = big_pow((seq_t)4, (seq_t)n);
    pos += mul_small(t, 84);

    t = big_pow((seq_t)2, (seq_t)n);
    pos += mul_small(t, 9);

    seq_t neg = mul_small(big_pow((seq_t)9, (seq_t)n), 9);
    neg += mul_small(big_pow((seq_t)7, (seq_t)n), 84);
    neg += mul_small(big_pow((seq_t)5, (seq_t)n), 126);
    neg += mul_small(big_pow((seq_t)3, (seq_t)n), 36);
    neg += 1;

    return pos - neg;
}

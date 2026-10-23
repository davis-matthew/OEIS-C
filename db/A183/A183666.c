/*
 * A183666  Number of (n+1) X 4 0..6 arrays with every 2 X 2 subblock summing
 *          to 12.
 * https://oeis.org/A183666
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*7^n + 84*6^n + 250*5^n + 440*4^n + 582*3^n + 604*2^n +
 *            434, offset 1
 * Range:     u8 none, u16 n <= 2, u32 n <= 9, u64 n <= 21, u128 n <= 44,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    /* n is 1-based index; compute using the closed‑form expression */
    seq_t pow7 = big_pow((seq_t)7, (seq_t)n);
    seq_t pow6 = big_pow((seq_t)6, (seq_t)n);
    seq_t pow5 = big_pow((seq_t)5, (seq_t)n);
    seq_t pow4 = big_pow((seq_t)4, (seq_t)n);
    seq_t pow3 = big_pow((seq_t)3, (seq_t)n);
    seq_t pow2 = big_pow((seq_t)2, (seq_t)n);

    seq_t res = 0;
    res += (seq_t)7   * pow7;
    res += (seq_t)84  * pow6;
    res += (seq_t)250 * pow5;
    res += (seq_t)440 * pow4;
    res += (seq_t)582 * pow3;
    res += (seq_t)604 * pow2;
    res += (seq_t)434;
    return res;
}

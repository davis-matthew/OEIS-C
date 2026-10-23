/*
 * A183643  Number of (n+1) X (n+1) 0..4 arrays with every 2 X 2 subblock
 *          summing to 8.
 * https://oeis.org/A183643
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*9^n + 16*8^n - 24*6^n + 10*5^n - 12*4^n + 6*3^n
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t pow3 = big_pow((seq_t)3, sn);
    seq_t pow4 = big_pow((seq_t)4, sn);
    seq_t pow5 = big_pow((seq_t)5, sn);
    seq_t pow6 = big_pow((seq_t)6, sn);
    seq_t pow8 = big_pow((seq_t)8, sn);
    seq_t pow9 = big_pow((seq_t)9, sn);

    seq_t res = 0;
    res += (seq_t)9  * pow9;
    res += (seq_t)16 * pow8;
    res += (seq_t)10 * pow5;
    res += (seq_t)6  * pow3;
    res -= (seq_t)24 * pow6;
    res -= (seq_t)12 * pow4;
    return res;
}

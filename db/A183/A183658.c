/*
 * A183658  Number of (n+1) X 6 0..5 arrays with every 2 X 2 subblock summing
 *          to 10.
 * https://oeis.org/A183658
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*6^n + 310*5^n + 2408*4^n + 8106*3^n + 16324*2^n + 19502
 * Range:     u8 none, u16 none, u32 n <= 9, u64 n <= 23, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline seq_t my_pow(seq_t base, idx_t exp)
{
    seq_t res = (seq_t)1;
    while (exp)
    {
        if (exp & 1)
            res *= base;
        base *= base;
        exp >>= 1;
    }
    return res;
}

seq_t a(idx_t n)
{
    seq_t p2 = my_pow((seq_t)2, n);
    seq_t p3 = my_pow((seq_t)3, n);
    seq_t p4 = my_pow((seq_t)4, n);
    seq_t p5 = my_pow((seq_t)5, n);
    seq_t p6 = my_pow((seq_t)6, n);
    seq_t res = (seq_t)19502;
    res += (seq_t)16324 * p2;
    res += (seq_t)8106 * p3;
    res += (seq_t)2408 * p4;
    res += (seq_t)310 * p5;
    res += (seq_t)6 * p6;
    return res;
}

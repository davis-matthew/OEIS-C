/*
 * A183675  Number of (n+1) X 3 0..7 arrays with every 2 X 2 subblock summing
 *          to 14.
 * https://oeis.org/A183675
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*8^n + 42*7^n + 72*6^n + 90*5^n + 96*4^n + 90*3^n +
 *            72*2^n + 42
 * Range:     u8 none, u16 n <= 3, u32 n <= 9, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   pow_small_base
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_small_base.h"

seq_t a(idx_t n)
{
    seq_t res = 0;
    seq_t p;

    /* 8 * 8^n */
    p = pow_small_base(8, n);
    res += (seq_t)8 * p;

    /* 42 * 7^n */
    p = pow_small_base(7, n);
    res += (seq_t)42 * p;

    /* 72 * 6^n */
    p = pow_small_base(6, n);
    res += (seq_t)72 * p;

    /* 90 * 5^n */
    p = pow_small_base(5, n);
    res += (seq_t)90 * p;

    /* 96 * 4^n */
    p = pow_small_base(4, n);
    res += (seq_t)96 * p;

    /* 90 * 3^n */
    p = pow_small_base(3, n);
    res += (seq_t)90 * p;

    /* 72 * 2^n */
    p = pow_small_base(2, n);
    res += (seq_t)72 * p;

    /* constant term 42 */
    res += 42;

    return res;
}

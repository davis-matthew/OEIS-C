/*
 * A183644  Number of (n+1) X 2 0..4 arrays with every 2 X 2 subblock summing
 *          to 8.
 * https://oeis.org/A183644
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 + 2^(n+2) + 2^(3+2*n) + 2*3^(n+1) + 5^(n+1)
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 12, u64 n <= 26, u128 n <= 54,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    seq_t val = 2;
    val += seq_pow((seq_t)2, n + 2);
    val += seq_pow((seq_t)2, 3 + (2 * n));
    val += seq_pow((seq_t)3, n + 1) * (seq_t)2;
    val += seq_pow((seq_t)5, n + 1);
    return val;
}

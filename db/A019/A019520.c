/*
 * A019520  a(n) is the concatenation of the first n positive even numbers.
 * https://oeis.org/A019520
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy, less
 * Formula:   a(n) = concatenation of the first n positive even numbers
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   digit_len
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/digit_len.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    seq_t result = 0;
    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t even_val = (seq_t)2 * (seq_t)i;
        int d = digit_len(even_val);          /* number of decimal digits */
        seq_t mult = (seq_t)1;
        for (int j = 0; j < d; ++j)
            mult *= (seq_t)10;                /* 10^d */
        result = result * mult + even_val;
    }
    return result;
}

/*
 * A128016  Expansion of (1+x+x^2+x^3)/(1-x^2+x^4).
 * https://oeis.org/A128016
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = periodic sequence of period 12:
 *            [1,1,2,2,1,1,-1,-1,-2,-2,-1,-1] with a(n)=value at (n mod 12)
 * Range:     i8 n <= 71, i16 n <= 71, i32 n <= 71, i64 n <= 71, i128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
seq_t a(idx_t n)
{
    static const seq_t table[12] = { (seq_t)1, (seq_t)1, (seq_t)2, (seq_t)2, (seq_t)1, (seq_t)1,
                                    (seq_t)-1, (seq_t)-1, (seq_t)-2, (seq_t)-2, (seq_t)-1, (seq_t)-1 };
    return table[n % 12];
}

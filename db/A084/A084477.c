/*
 * A084477  Number of fault-free tilings of a 4 X 3n rectangle with right
 *          trominoes.
 * https://oeis.org/A084477
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8 * 6^(n-3) for n > 2 Initial values: a(1)=4, a(2)=2,
 *            a(3)=8 a(n) = 6 * a(n-1) for n > 3 is equivalent
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 14, u64 n <= 26, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* handle small indices directly */
    if (n == (idx_t)1)
        return (seq_t)4;
    if (n == (idx_t)2)
        return (seq_t)2;
    if (n == (idx_t)3)
        return (seq_t)8;

    /* compute 6^(n-3) using exponentiation by squaring */
    seq_t pow6 = (seq_t)1;
    seq_t base = (seq_t)6;
    idx_t exp = n - (idx_t)3;
    while (exp > (idx_t)0) {
        if (exp & (idx_t)1)
            pow6 *= base;
        base *= base;
        exp >>= (idx_t)1;
    }

    return (seq_t)8 * pow6;
}

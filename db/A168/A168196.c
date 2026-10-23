/*
 * A168196  a(n) = n^4*(n^5 + 1)/2.
 * https://oeis.org/A168196
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = n^4*(n^5 + 1)/2
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 12, u64 n <= 149, u128 n <= 405,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;
    seq_t n2 = tn * tn;
    seq_t n3 = n2 * tn;
    seq_t n4 = n3 * tn;
    seq_t n5 = n4 * tn;
    seq_t nu = n5 + (seq_t)1;

    /* One of n4 or nu is even; divide the even one by 2 first */
    if ((n4 % (seq_t)2) == 0)
        return (n4 / (seq_t)2) * nu;
    else
        return n4 * (nu / (seq_t)2);
}

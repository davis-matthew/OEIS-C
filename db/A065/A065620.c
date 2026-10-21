/*
 * A065620  a(0)=0; thereafter a(2n) = 2a(n), a(2n+1) = -2a(n) + 1.
 * https://oeis.org/A065620
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, sign, look
 * Formula:   a(0)=0; a(2n)=2*a(n); a(2n+1)=-2*a(n)+1
 * Range:     i8 n <= 127, i16 n <= 9745, i32 n <= 9745, i64 n <= 9745,
 *            i128 n <= 9745, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* base case */

    seq_t result = 0;
    int sign = 1;                          /* alternates starting with + */

    /* iterate over set bits of n from least‑significant to most‑significant */
    while (n != 0) {
        /* isolate lowest set bit: 1<<pos */
        uint64_t mask = n & -n;            /* lowest power of two in n   */
        uint64_t pos  = __builtin_ctzll(mask);   /* position of that bit */

        seq_t term = ((seq_t)1) << pos;    /* 2^pos in type seq_t */

        if (sign > 0)
            result += term;
        else
            result -= term;

        n -= mask;          /* clear the processed bit   */
        sign = -sign;       /* flip sign for next bit    */
    }

    return result;
}

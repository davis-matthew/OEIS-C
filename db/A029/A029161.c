/*
 * A029161  Expansion of 1/((1-x^2)*(1-x^3)*(1-x^8)*(1-x^9)).
 * https://oeis.org/A029161
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n^3+33*n^2+300*n+1120)/2592 * - (n+5)*(n mod 2)/32
 *            * + n*((n^2+n+1) mod 3)/27 * + ((n+2) mod 3)/3) * (implemented
 *            by bringing all terms to a common denominator 2592)
 * Range:     u8 none, u16 n <= 29, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;

    /* terms of the numerator in a common denominator 2592 */
    seq_t num1 = N*N*N + 33*N*N + 300*N + 1120;           /* /2592    */
    seq_t num2 = 81 * (N + 5) * (n % 2);                  /* /2592    */
    seq_t num3 = 96 * N * ((N*N + N + 1) % 3);            /* /2592    */
    seq_t num4 = 864 * ((N + 2) % 3);                    /* /2592    */

    seq_t num = num1 - num2 + num3 + num4;
    return num / 2592;
}

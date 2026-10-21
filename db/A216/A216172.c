/*
 * A216172  Number of all possible tetrahedra of any size, having reverse
 *          orientation to the original regular tetrahedron, formed when
 *          intersecting the latter by planes parallel to its sides and
 *          dividing its edges into n equal parts.
 * https://oeis.org/A216172
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/72)*(-6*n -5*n^2 + 2*n^3 + n^4 + 4 - 4*(-1)^(n mod 3))
 * Range:     u8 n <= 11, u16 n <= 46, u32 n <= 745, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computes the nth term of A216172 (offset 1). */
seq_t a(idx_t n) {
    /* Use 128‑bit signed arithmetic to avoid intermediate underflow.    */
    /* GCC/Clang provide __int128 in both C11 and C++17 modes.            */
    __int128 nn = ( __int128 )(n);            /* convert n to signed 128  */
    __int128 t  = nn * nn * nn * nn;          /* n^4                           */
    t += ( __int128 )2 * nn * nn * nn;        /* + 2 n^3                       */
    t -= ( __int128 )5 * nn * nn;            /* - 5 n^2                       */
    t -= ( __int128 )6 * nn;                 /* - 6 n                         */
    t += 4;                                  /* + 4                            */
    /* term 4*(-1)^(n mod 3). */
    int sign = ( (n % 3) == 1 ) ? -1 : 1;    /* (-1)^(n mod 3) */
    t -= 4 * sign;                           /* - 4 * sign                   */
    /* The result is a multiple of 72. */
    return (seq_t)(t / 72);
}

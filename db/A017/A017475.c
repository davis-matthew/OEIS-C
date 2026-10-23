/*
 * A017475  a(n) = (11*n + 7)^3.
 * https://oeis.org/A017475
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11*n + 7)^3
 * Range:     u8 none, u16 n <= 3, u32 n <= 147, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t t = (seq_t)n;
    t = t * (seq_t)11 + (seq_t)7;  /* t = 11*n + 7 */
    seq_t cube = t * t;            /* t^2 */
    cube = cube * t;               /* (11*n + 7)^3 */
    return cube;
}

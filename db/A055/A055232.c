/*
 * A055232  Expansion of (1+2*x+3*x^2)/((1-x)^3*(1-x^2)).
 * https://oeis.org/A055232
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7 + (-1)^n + 16*n + 14*n^2 + 4*n^3) / 8
 * Range:     u8 n <= 2, u16 n <= 24, u32 n <= 999, u64 n <= 999,
 *            u128 n <= 999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Convert index to seq_t for arithmetic */
    seq_t sn = (seq_t)n;
    /* Compute powers of n in seq_t */
    seq_t n2 = sn * sn;       /* n^2 */
    seq_t n3 = n2 * sn;       /* n^3 */

    /* (-1)^n */
    seq_t neg1pow = (n & 1) ? (seq_t)-1 : (seq_t)1;

    /* Numerator: 7 + (-1)^n + 16*n + 14*n^2 + 4*n^3 */
    seq_t numerator = (seq_t)7 + neg1pow
                     + (seq_t)16 * sn
                     + (seq_t)14 * n2
                     + (seq_t)4  * n3;

    /* Divide by 8 */
    return numerator / (seq_t)8;
}

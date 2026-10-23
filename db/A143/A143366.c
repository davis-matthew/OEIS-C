/*
 * A143366  Wiener index of the hexagon crown (beehive model) with n hexagons
 *          on each side of the outside ring.
 * https://oeis.org/A143366
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(164*n^4-30*n^2+1)/5
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 25, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n4 = n2 * n2;
    seq_t term = 164 * n4 - 30 * n2 + (seq_t)1;
    if (n % 5 == 0) {
        seq_t n_div = nn / (seq_t)5;
        return term * n_div;
    } else {
        seq_t term_div = term / (seq_t)5;
        return term_div * nn;
    }
}

/*
 * A117216  Number of points in the standard root system version of the D_4
 *          lattice having L_infinity norm n.
 * https://oeis.org/A117216
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*n*(1+4*n^2) for n>0; a(0)=1
 * Range:     u8 n <= 1, u16 n <= 12, u32 n <= 511, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    seq_t nn = (seq_t)n;
    seq_t nn_sq = nn * nn;
    seq_t term = (seq_t)1 + (seq_t)4 * nn_sq;
    return (seq_t)8 * nn * term;
}

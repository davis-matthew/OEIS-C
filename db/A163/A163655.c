/*
 * A163655  a(n) = n*(2*n^2 + 5*n + 13)/2.
 * https://oeis.org/A163655
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(2*n^2 + 5*n + 13)/2
 * Range:     u8 n <= 5, u16 n <= 39, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t ns = (seq_t)n;
    seq_t n_sq = ns * ns;                       /* n^2 */
    seq_t term = ( (seq_t)2 * n_sq ) + ( (seq_t)5 * ns ) + (seq_t)13;
    return (ns * term) / ( (seq_t)2 );
}

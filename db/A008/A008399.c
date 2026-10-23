/*
 * A008399  Coordination sequence for E_6 lattice.
 * https://oeis.org/A008399
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*n*(13*n^2+7)*(n^2+1)/5, with a(0) = 1
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 44, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    unsigned __int128 N = (unsigned __int128)n;
    unsigned __int128 val = 9 * N * (13 * N * N + 7) * (N * N + 1) / 5;
    return (seq_t)val;
}

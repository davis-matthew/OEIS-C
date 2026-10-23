/*
 * A195163  1000-gonal numbers: a(n) = n*(499*n - 498).
 * https://oeis.org/A195163
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(499*n - 498)
 * Range:     u8 n <= 1, u16 n <= 11, u32 n <= 2934, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t sn = (seq_t)n;
    return sn * ((seq_t)499 * sn - (seq_t)498);
}

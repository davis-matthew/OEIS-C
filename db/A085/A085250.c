/*
 * A085250  4 times hexagonal numbers: a(n) = 4*n*(2*n-1).
 * https://oeis.org/A085250
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n*(2*n-1)
 * Range:     u8 n <= 5, u16 n <= 90, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) {
        return 0;
    }
    seq_t tn = (seq_t)n;
    seq_t prod = tn * ((seq_t)2 * tn - 1);
    return prod * 4;
}

/*
 * A027620  a(n) = n + (n+1)^2 + (n+2)^3.
 * https://oeis.org/A027620
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*(n+3)^2
 * Range:     u8 n <= 4, u16 n <= 37, u32 n <= 1623, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t n1 = (seq_t)n + 1;
    seq_t n3 = (seq_t)n + 3;
    seq_t sq = n3 * n3;
    return n1 * sq;
}

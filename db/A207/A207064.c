/*
 * A207064  Number of n X 4 0..1 arrays avoiding 0 0 1 and 0 1 0 horizontally
 *          and 0 0 1 and 1 0 1 vertically.
 * https://oeis.org/A207064
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (3/4)*n^4 + (15/2)*n^3 + (15/4)*n^2 - 3*n Equivalently:
 *            a(n) = (3*n^4 + 30*n^3 + 15*n^2 - 12*n) / 4 for n ≥ 1
 * Range:     u8 n <= 1, u16 n <= 10, u32 n <= 198, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;  /* Offset starts at 1; n==0 is undefined */
    seq_t N  = (seq_t)n;
    seq_t n2 = N * N;
    seq_t n3 = n2 * N;
    seq_t n4 = n3 * N;
    seq_t sum = (seq_t)3 * n4
              + (seq_t)30 * n3
              + (seq_t)15 * n2
              - (seq_t)12 * N;
    return sum / (seq_t)4;
}

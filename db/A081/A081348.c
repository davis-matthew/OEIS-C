/*
 * A081348  First row in maze arrangement of natural numbers.
 * https://oeis.org/A081348
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (2*n^2+4*n+3-(2*n+1)*(-1)^n)/2
 * Range:     u8 n <= 14, u16 n <= 254, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t n_t = (seq_t)n;
    if ((n & 1) == 0) {
        return n_t * n_t + n_t + 1;          /* even n: n^2 + n + 1 */
    } else {
        return n_t * n_t + 3 * n_t + 2;      /* odd  n: n^2 + 3*n + 2 */
    }
}

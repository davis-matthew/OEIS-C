/*
 * A082532  a(n) = n^2 - 2*floor(n/sqrt(2))^2.
 * https://oeis.org/A082532
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n^2 - 2*floor(n/sqrt(2))^2
 * Range:     u8 n <= 7, u16 n <= 31, u32 n <= 8385, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/isqrt_seq.h"

seq_t a(idx_t n) {
    /* Convert index to seq_t once */
    seq_t ns = (seq_t)n;
    /* Compute n^2 */
    seq_t n2 = ns * ns;
    /* floor(n^2 / 2) */
    seq_t half = n2 / 2;
    /* floor(sqrt( floor(n^2 / 2) )) */
    seq_t k = isqrt_seq(half);
    /* Return n^2 - 2*k^2 */
    return n2 - (seq_t)2 * k * k;
}

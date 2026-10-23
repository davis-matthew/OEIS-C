/*
 * A184586  a(n) = floor((n-1/2)*r), where r=sqrt(5); complement of A184587.
 * https://oeis.org/A184586
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((n-1/2)*sqrt(5))
 * Range:     u8 n <= 4, u16 n <= 57, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Helpers:   _isqrt_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/_isqrt_seq_t.h"

seq_t a(idx_t n) {
    /* n is 1‑based; compute m = 2n-1 (odd) */
    idx_t m = (idx_t)2 * n - 1;

    /* K = 5 * m^2, fits in seq_t for the ranges considered */
    seq_t K = (seq_t)5 * (seq_t)m * (seq_t)m;

    /* floor(sqrt(K)) / 2 gives a(n) */
    seq_t s = _isqrt_seq_t(K);
    return s >> 1;
}

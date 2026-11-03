/*
 * A213572  Principal diagonal of the convolution array A213571.
 * https://oeis.org/A213572
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2^(n+2)*(2^n-1) - (2^(n+1) + n + 1)*n)/2
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 970 (2026-11-01)
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;            /* offset 1; below offset undefined */

    seq_t n_seq = (seq_t)n;
    seq_t two_n   = pow2(n);
    seq_t two_n1  = pow2(n + 1);
    seq_t two_n2  = pow2(n + 2);

    seq_t partA = two_n2 * (two_n - 1);
    seq_t partB = (two_n1 + n_seq + 1) * n_seq;

    return (partA - partB) / 2;
}

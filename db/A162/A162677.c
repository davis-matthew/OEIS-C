/*
 * A162677  Number of different fixed (possibly) disconnected polyominoes (of
 *          any area) bounded (not necessarily tightly) by an n*n square.
 * https://oeis.org/A162677
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(n^2) - 2*2^((n-1)*n) + 2^((n-1)^2)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 5, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n) {
    // the function is valid for all n >= 1 (and also works for n = 0)
    idx_t n1 = n;
    idx_t n2 = n - 1;
    idx_t exp1 = n1 * n1;
    idx_t exp2 = n2 * n1;
    idx_t exp3 = n2 * n2;
    seq_t term1 = pow2(exp1);
    seq_t term2 = pow2(exp2);
    seq_t term3 = pow2(exp3);
    seq_t result = term1;
    result -= term2;  // subtract 2^((n-1)*n)
    result -= term2;  // second subtraction for the factor 2
    result += term3;  // add 2^((n-1)^2)
    return result;
}

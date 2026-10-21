/*
 * A235891  Number of (n+1) X (6+1) 0..3 arrays with the minimum plus the
 *          upper median equal to the lower median plus the maximum in every 2
 *          X 2 subblock.
 * https://oeis.org/A235891
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*(3906 + 189*2^n + 4^n) (for n ≥ 1)
 * Range:     u8 none, u16 n <= 5, u32 n <= 14, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   mul_small, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n) {
    /* Out of range values are not defined; return 0 for safety. */
    if (n == 0) return 0;

    /* 2^n */
    seq_t two_pow_n = pow2(n);

    /* 189 * 2^n */
    seq_t term2 = mul_small(two_pow_n, 189);

    /* 4^n = 2^(2n) */
    seq_t four_pow_n = pow2(2 * n);

    /* sum = 3906 + term2 + four_pow_n */
    seq_t sum = 3906 + term2 + four_pow_n;

    /* a(n) = 4 * sum */
    return sum * 4;
}

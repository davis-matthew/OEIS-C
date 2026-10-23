/*
 * A027639  Order of unitary 2^n X 2^n group H_{n,4} acting on Siegel modular
 *          forms.
 * https://oeis.org/A027639
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = 2^(n^2 + 2*n + 2) * ∏_{j=1..n} (2^j - 1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 8,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* exponent n^2 + 2*n + 2 */
    idx_t exp = n * n + 2 * n + 2;

    /* 2^(exp) as seq_t */
    seq_t two_pow_exp = pow2(exp);

    /* product of (2^j - 1) for j = 1..n */
    seq_t prod = 1;
    for (idx_t j = 1; j <= n; ++j) {
        seq_t two_pow_j = pow2(j);   /* 2^j */
        seq_t term = two_pow_j - 1;  /* 2^j - 1 */
        prod *= term;
    }

    return two_pow_exp * prod;
}

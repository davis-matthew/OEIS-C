/*
 * A052317  Number of labeled trees with n nodes and 2-colored internal
 *          (non-leaf) nodes.
 * https://oeis.org/A052317
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{j=0}^n (-1)^{n-j} * C(n,j) * 2^j * j^{n-2} for n ≥ 2
 *            a(0) = a(1) = 1 (derived from the egf 1 + B(x) – x*B(x) –
 *            B(x)^2/2
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    if (n == 0 || n == 1) {
        return 1;
    }

    /* exponent in the power j^(n-2) */
    idx_t exp = n - 2;

    seq_t sum_even = 0;
    seq_t sum_odd  = 0;

    seq_t binom = 1;      /* C(n,0) */
    seq_t pow2  = 1;      /* 2^0 */

    for (idx_t j = 0; j <= n; ++j) {
        /* compute j^exp */
        seq_t j_pow;
        if (j == 0) {
            /* 0^0 = 1, 0^k = 0 for k>0 */
            j_pow = (exp == 0) ? 1 : 0;
        } else {
            j_pow = pow_uint((uint64_t)j, exp);
        }

        seq_t term = binom * j_pow;
        term = term * pow2;

        if (((n - j) & 1) == 1) {
            sum_odd += term;
        } else {
            sum_even += term;
        }

        /* prepare for next j (unless j == n) */
        if (j == n) {
            break;
        }
        /* C(n,j+1) = C(n,j) * (n-j) / (j+1) */
        binom = binom * (seq_t)(n - j) / (seq_t)(j + 1);
        pow2 = pow2 * 2;
    }

    /* a(n) is non‑negative, compute difference as a positive value */
    if (sum_even >= sum_odd) {
        return sum_even - sum_odd;
    } else {
        /* theoretically shouldn't happen for n ≥ 2, but keep it robust */
        return sum_odd - sum_even;
    }
}

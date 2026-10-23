/*
 * A034735  Dirichlet convolution of b_n=2^(n-1) with c_n=3^(n-1).
 * https://oeis.org/A034735
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} 2^(d-1) * 3^(n/d-1) (Dirichlet convolution of
 *            2^(n-1) and 3^(n-1))
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 21, u64 n <= 41, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Helper: fast exponentiation for small bases */
static seq_t pow_small(idx_t base_val, idx_t exp) {
    seq_t result = 1;
    seq_t base = (seq_t)base_val;
    while (exp) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    seq_t ans = 0;
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            /* divisor d = i */
            idx_t d1 = i;
            seq_t term1 = pow_small(2, d1 - 1) * pow_small(3, n / d1 - 1);
            ans += term1;

            /* second divisor d = n / i, if distinct */
            idx_t d2 = n / i;
            if (d2 != d1) {
                seq_t term2 = pow_small(2, d2 - 1) * pow_small(3, d1 - 1);
                ans += term2;
            }
        }
    }
    return ans;
}

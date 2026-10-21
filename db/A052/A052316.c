/*
 * A052316  Number of labeled rooted trees with n nodes and 2-colored internal
 *          (non-leaf) nodes.
 * https://oeis.org/A052316
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, eigen
 * Formula:   a(n) = ∑_{j=1..n} (-1)^(n-j) * binomial(n,j) * j^(n-1) * 2^j
 *            (OEIS A052316)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 14, u128 n <= 17,
 *            big any n
 * Helpers:   mul_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mul_big.h"

/* fast exponentiation returning a seq_t. */
static seq_t pow_seq(idx_t base_int, idx_t exponent)
{
    seq_t result = 1;
    seq_t base = (seq_t)base_int;
    while (exponent > 0) {
        if (exponent & 1)
            result = mul_big(result, base);
        base = mul_big(base, base);
        exponent >>= 1;
    }
    return result;
}

/* sequence A052316: number of labeled rooted trees with n nodes and 2‑colored
   internal nodes.  Offset is 1. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset not defined – harmless */
    if (n == 1) return 1;          /* initial condition */

    seq_t sum_pos = 0;
    seq_t sum_neg = 0;
    seq_t bin      = 1;            /* C(n,0) */
    seq_t two_pow  = 1;            /* 2^0 */

    for (idx_t j = 1; j <= n; ++j) {
        /* Update binomial coefficient C(n,j) */
        bin   = mul_big(bin, (seq_t)(n - j + 1));
        bin   = bin / (seq_t)j;     /* division is exact */

        /* Update 2^j */
        two_pow = mul_big(two_pow, (seq_t)2);

        /* Compute j^(n-1) */
        seq_t pow_term = pow_seq(j, n - 1);

        /* term = bin * 2^j * j^(n-1) */
        seq_t term = mul_big(bin, two_pow);
        term = mul_big(term, pow_term);

        /* Alternate signs: (-1)^(n-j) */
        if (((n - j) & 1) == 0) {   /* even → positive */
            sum_pos += term;
        } else {                   /* odd → negative */
            sum_neg += term;
        }
    }

    /* Final result: positive part minus negative part */
    return sum_pos - sum_neg;
}

/*
 * A100980  Number of totally ramified extensions over Q_3 with degree n in
 *          the algebraic closure of Q_3.
 * https://oeis.org/A100980
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * Σ_{s=0}^{m} 3^s * (3^{(n - n/3^s)/2} - 3^{(n -
 *            n/3^{s-1})/2}), where m = v_3(n) (the exponent of 3 in n) and
 *            3^(-∞)=0. Note that (n - n/3^s) is always even, so the exponents
 *            are integers
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 35, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   pow_n
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_n.h"

seq_t a(idx_t n)
{
    /* Special case: n >= 1 is guaranteed. */
    /* Compute the exponent m of 3 in n. */
    idx_t temp = n;
    idx_t m = 0;
    while (temp % 3 == 0) {
        temp /= 3;
        ++m;
    }

    seq_t sum = 0;
    idx_t pow3s = 1;          /* 3^s as idx_t (fits easily) */
    seq_t pow3s_seq = 1;      /* same value as seq_t for multiplication */

    for (idx_t s = 0; s <= m; ++s) {
        /* denominator n / 3^s */
        idx_t denom = n / pow3s;
        /* exponent exp_s = (n - n/3^s) / 2, guaranteed integer */
        idx_t exp_s = (n - denom) / 2;
        seq_t term_pow = pow_n((seq_t)3, exp_s);

        seq_t term_pow_prev = 0;      /* 3^(eps(-1)*n) -> 0 for s=0 */
        if (s > 0) {
            idx_t pow3s_prev = pow3s / 3;      /* 3^{s-1} */
            idx_t denom_prev = n / pow3s_prev;
            idx_t exp_prev = (n - denom_prev) / 2;
            term_pow_prev = pow_n((seq_t)3, exp_prev);
        }

        seq_t inner = term_pow - term_pow_prev;
        seq_t term  = pow3s_seq * inner;

        sum += term;

        /* Prepare for next s */
        pow3s *= 3;
        pow3s_seq = (seq_t)pow3s;
    }

    return (seq_t)n * sum;
}

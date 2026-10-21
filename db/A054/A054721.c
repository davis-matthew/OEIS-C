/*
 * A054721  Number of 6-ary sequences with primitive period n.
 * https://oeis.org/A054721
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} mu(d) * 6^(n/d), with a(0)=1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   mu_of, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/mu_of.h"
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;   /* a(0) = 1 */

    seq_t sum_pos = 0;
    seq_t sum_neg = 0;

    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d != 0) continue;

        /* divisor d */
        int mu_d  = mu_of(d);
        idx_t exp_d  = n / d;
        seq_t p_d = pow_seq((seq_t)6, (int)exp_d);

        if (mu_d > 0)   sum_pos += p_d;
        else if (mu_d < 0) sum_neg += p_d;

        /* paired divisor n/d (skip if same) */
        idx_t e = d;
        idx_t f = n / d;
        if (e != f) {
            int mu_f  = mu_of(f);
            idx_t exp_f  = n / f;   /* which is d */
            seq_t p_f = pow_seq((seq_t)6, (int)exp_f);

            if (mu_f > 0)   sum_pos += p_f;
            else if (mu_f < 0) sum_neg += p_f;
        }
    }

    return sum_pos - sum_neg;
}

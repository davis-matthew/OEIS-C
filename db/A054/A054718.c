/*
 * A054718  Number of ternary sequences with primitive period n.
 * https://oeis.org/A054718
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} mu(d)*3^(n/d)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   big_pow, mu_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mu_of.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)0) {
        return (seq_t)1;
    }

    seq_t pos = (seq_t)0;
    seq_t neg = (seq_t)0;

    for (idx_t d = (idx_t)1; d * d <= n; ++d) {
        if (n % d == (idx_t)0) {
            int mu_d = mu_of(d);
            if (mu_d != 0) {
                seq_t term = big_pow((seq_t)3, (seq_t)(n / d));
                if (mu_d == 1) {
                    pos += term;
                } else { /* mu_d == -1 */
                    neg += term;
                }
            }

            idx_t e = n / d;
            if (e != d) {
                int mu_e = mu_of(e);
                if (mu_e != 0) {
                    seq_t term = big_pow((seq_t)3, (seq_t)(n / e)); /* exponent equals d */
                    if (mu_e == 1) {
                        pos += term;
                    } else { /* mu_e == -1 */
                        neg += term;
                    }
                }
            }
        }
    }

    return pos >= neg ? pos - neg : (seq_t)0;
}

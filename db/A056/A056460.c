/*
 * A056460  Number of primitive (aperiodic) palindromes using a maximum of
 *          four different symbols.
 * https://oeis.org/A056460
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} mu(d) * 4^ceil((n/d)/2) (OEIS A056460)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   mu_of_idx, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>
#include "../utils/seq_pow.h"
#include "../utils/mu_of_idx.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    seq_t sum = 0;
    idx_t sqrt_n = (idx_t)(sqrt((double)n) + 0.5);

    for (idx_t i = 1; i <= sqrt_n; ++i) {
        if (n % i == 0) {
            /* divisor d1 = i */
            idx_t d1 = i;
            int mu1 = mu_of_idx(d1);
            if (mu1) {
                idx_t k = n / d1;
                idx_t e = (k + 1) / 2;          /* ceil(k/2) */
                seq_t term = seq_pow((seq_t)4, e);
                if (mu1 > 0) sum += term;
                else        sum -= term;
            }

            /* divisor d2 = n / i, if distinct */
            idx_t d2 = n / i;
            if (d2 != d1) {
                int mu2 = mu_of_idx(d2);
                if (mu2) {
                    idx_t k = n / d2;
                    idx_t e = (k + 1) / 2;
                    seq_t term = seq_pow((seq_t)4, e);
                    if (mu2 > 0) sum += term;
                    else        sum -= term;
                }
            }
        }
    }

    return sum;
}

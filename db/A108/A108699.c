/*
 * A108699  a(n) = Product{k=1 to n} sigma_{n-k+1}(k), where sigma_m(k) =
 *          sum{j|k} j^m.
 * https://oeis.org/A108699
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{k=1}^{n} σ_{n-k+1}(k), where σ_m(k) = Σ_{d|k} d^m
 *            (OEIS A108699)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 6, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    seq_t product = 1;

    for (idx_t k = 1; k <= n; ++k) {
        /* exponent for this term */
        idx_t m = n - k + 1;

        seq_t sigma = 0;

        /* Enumerate divisors of k */
        for (idx_t d = 1; d * d <= k; ++d) {
            if (k % d == 0) {
                /* d^m */
                sigma += pow_seq((seq_t)d, m);

                idx_t q = k / d;
                if (q != d) {
                    /* (k/d)^m */
                    sigma += pow_seq((seq_t)q, m);
                }
            }
        }
        product *= sigma;
    }

    return product;
}

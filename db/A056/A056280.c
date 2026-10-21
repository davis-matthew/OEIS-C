/*
 * A056280  Number of primitive (aperiodic) word structures of length n which
 *          contain exactly four different symbols.
 * https://oeis.org/A056280
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} μ(d) * S(n/d,4) (S(m,4) = Stirling number of
 *            the second kind)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 34, u128 n <= 66,
 *            big any n
 * Helpers:   mu_of, stirling4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/mu_of.h"
#include "../utils/stirling4.h"

seq_t a(idx_t n)
{
    seq_t sum_pos = 0, sum_neg = 0;
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i != 0) continue;
        idx_t d1 = i;
        idx_t d2 = n / i;

        int mu1 = mu_of(d1);
        idx_t m1 = n / d1;
        if (mu1 == 1)
            sum_pos += stirling4(m1);
        else if (mu1 == -1)
            sum_neg += stirling4(m1);

        if (d1 != d2) {
            int mu2 = mu_of(d2);
            idx_t m2 = n / d2;   /* equals i */
            if (mu2 == 1)
                sum_pos += stirling4(m2);
            else if (mu2 == -1)
                sum_neg += stirling4(m2);
        }
    }
    return sum_pos - sum_neg;
}

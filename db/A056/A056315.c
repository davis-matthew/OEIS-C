/*
 * A056315  Number of primitive (aperiodic) reversible strings with n beads
 *          using a maximum of four different colors.
 * https://oeis.org/A056315
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d|n} μ(d) * (4^{n/d} + 4^{ceil((n/d)/2)}) / 2
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   mu_of, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mu_of.h"   /* int mu_of(idx_t d); */
#include "../utils/pow4.h"    /* seq_t pow4(idx_t exp); */

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* n < 1 is out of range */

    seq_t sum = 0;

    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d != 0) continue;

        /* divisor d */
        int mu_d = mu_of(d);
        if (mu_d != 0) {
            idx_t m = n / d;                     /* m = n/d */
            seq_t term = (pow4(m) + pow4((m + 1) / 2)) / 2;
            if (mu_d == 1) sum += term;
            else           sum -= term;
        }

        /* paired divisor e = n/d, avoid double counting when d == e */
        idx_t e = n / d;
        if (e != d) {
            int mu_e = mu_of(e);
            if (mu_e != 0) {
                idx_t m = d;                     /* m = n/e = d */
                seq_t term = (pow4(m) + pow4((m + 1) / 2)) / 2;
                if (mu_e == 1) sum += term;
                else          sum -= term;
            }
        }
    }

    return sum;
}

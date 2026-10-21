/*
 * A217619  a(n) = m/(12*n) where m is the least multiple of n that satisfies
 *          phi(m) = phi(m+6*n).
 * https://oeis.org/A217619
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = m/(12*n) where m is the least multiple of n * satisfying
 *            φ(m) = φ(m + 6*n). (A217140(n)/12)
 * Range:     u8 n <= 8, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/phi.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                           /* undefined for n = 0 */

    /* Search over multiples of n */
    idx_t m = n;                          /* current candidate m */
    for (;; m += n) {
        /* φ(m) and φ(m + 6*n) */
        seq_t phi_m     = phi(m);
        seq_t phi_next  = phi(m + 6 * n);

        if (phi_m == phi_next) {
            /* m is divisible by 12*n, see OEIS; integer division works */
            seq_t denom = (seq_t)(12 * n);
            return ((seq_t)m) / denom;
        }
    }
}

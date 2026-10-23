/*
 * A023033  Number of compositions of n into 8 ordered relatively prime parts.
 * https://oeis.org/A023033
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 84, u64 n <= 1914,
 *            u128 n <= 5000, big any n
 * Helpers:   C_n_choose_7, mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A023033
   Formula: a(n) = Σ_{d|n} μ(d) * C(n/d – 1, 7)   for n ≥ 8
   (Moebius transform of C(n-1, 7))
*/

#include <stdint.h>
#include "../utils/mobius.h"
#include "../utils/C_n_choose_7.h"

seq_t a(idx_t n)
{
    /* Undefined for n < 8 – return 0 as a safe default. */
    if (n < 8) return (seq_t)0;

    __int128 total = 0;

    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            /* divisor d */
            int mu_d = (int)mobius(d);
            seq_t comb1 = C_n_choose_7(n / d - 1);
            total += (__int128)mu_d * (__int128)comb1;

            /* complementary divisor e = n/d, avoid double counting when d == e */
            idx_t e = n / d;
            if (e != d) {
                int mu_e = (int)mobius(e);
                seq_t comb2 = C_n_choose_7(n / e - 1); /* = C(d-1,7) */
                total += (__int128)mu_e * (__int128)comb2;
            }
        }
    }

    return (seq_t)total;
}

/*
 * A076369  a(n) = n + mu(n), where mu is the Moebius function (A008683).
 * https://oeis.org/A076369
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + mu(n)
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mu_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mu_of.h"

seq_t a(idx_t n)
{
    int mu = mu_of(n);           /* -1, 0, or 1 */
    seq_t res = (seq_t)n;        /* start with n */

    if (mu > 0) {
        res += 1;                /* μ(n) = 1 */
    } else if (mu < 0) {
        res -= 1;                /* μ(n) = -1 */
    }

    return res;                  /* a(n) = n + μ(n) */
}

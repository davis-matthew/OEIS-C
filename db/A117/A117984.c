/*
 * A117984  A modified Legendre-binomial transform of 4^n for p=3.
 * https://oeis.org/A117984
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=0..n} (-1)^(n-k) * L(C(n,k)/3) * 4^k, where L(j/3)
 *            is the Legendre symbol modulo 3: L(j/3) = 0 if j % 3 == 0, +1 if
 *            j % 3 == 1, -1 if j % 3 == 2
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 16, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   legendre_binom_mod3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/legendre_binom_mod3.h"

seq_t a(idx_t n)
{
    seq_t posSum = 0;
    seq_t negSum = 0;
    seq_t pow4   = 1;          /* 4^k, starts with k = 0 */

    /* Iterate over k = 0 .. n */
    for (idx_t k = 0; k <= n; ++k) {
        int sign = (((n - k) & 1) == 0) ? 1 : -1;   /* (-1)^(n-k) */
        int l    = legendre_binom_mod3(n, k);        /* Legendre symbol */

        if (l != 0) {
            int coeff = sign * l;                    /* coefficient -1, 0, or 1 */
            if (coeff == 1)
                posSum += pow4;
            else if (coeff == -1)
                negSum += pow4;
        }
        pow4 *= 4;        /* prepare next 4^k */
    }

    /* Since the final result is non‑negative, posSum >= negSum */
    return posSum - negSum;
}

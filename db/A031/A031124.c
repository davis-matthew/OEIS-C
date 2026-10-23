/*
 * A031124  Expansion of (1+z)/(1-z) - 2*Sum_{m>=1} z^(m^2)/(1-z^((m+1)^2)).
 * https://oeis.org/A031124
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (n==0 ? 1 : 2) - 2 * #{ m ≥ 1 : ∃ t ≥ 0 with n = m² +
 *            t·(m+1)² }
 * Range:     i8 n <= 79, i16 n <= 79, i32 n <= 79, i64 n <= 79, i128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This computes the coefficient of zⁿ in the expansion:
   (1+z)/(1‑z) – 2 ∑_{m≥1} z^{m²}/(1‑z^{(m+1)²})                          */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;                     /* a(0) = 1 */

    seq_t val = 2;                    /* base coefficient for n > 0 */

    for (idx_t m = 1; m * m <= n; ++m) {
        idx_t diff   = n - m * m;                 /* n – m²  */
        idx_t denom  = (m + 1) * (m + 1);         /* (m+1)²  */
        if (diff % denom == 0)                    /* representation exists? */
            val -= 2;                             /* subtract 2 per representation */
    }
    return val;
}

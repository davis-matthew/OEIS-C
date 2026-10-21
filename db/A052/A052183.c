/*
 * A052183  a(n) = (n + 2) * binomial(3*n, n) / (2*n + 1).
 * https://oeis.org/A052183
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n + 2) * binomial(3*n, n) / (2*n + 1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation computes binomial(3*n, n) multiplicatively,
   reducing intermediate products by gcd to keep the values within
   the limits of seq_t.  It then cancels factors with the denominator
   before the final multiplication. */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Convert n to seq_t for arithmetic. */
    seq_t nn = (seq_t)n;

    /* ---- Compute binomial(3*n, n) ---- */
    seq_t bin = 1;                    /* C(3*n, 0) = 1 */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t num = (seq_t)(2 * n + i);    /* numerator term: 2*n + i  (ranges 2n+1 … 3n) */
        seq_t den = (seq_t)i;              /* denominator term: i          */

        /* Reduce with current binomial result */
        seq_t g = gcd(bin, den);
        if (g > 1) {
            bin /= g;
            den /= g;
        }

        /* Reduce remaining denominator with numerator */
        g = gcd(num, den);
        if (g > 1) {
            num /= g;
            den /= g;
        }
        /* After reduction den should be 1 */
        bin = bin * num;
    }

    /* ---- Cancel factors with the final denominator ---- */
    seq_t den = nn * 2U + 1U;          /* 2*n + 1 */
    seq_t g = gcd(bin, den);
    bin /= g;
    den /= g;

    seq_t n_plus2 = nn + 2U;           /* n + 2 */
    g = gcd(n_plus2, den);
    n_plus2 /= g;
    den /= g;                          /* den should now be 1 */

    /* Final result */
    return bin * n_plus2 / den;        /* safe because den == 1 */
}

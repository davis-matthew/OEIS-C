/*
 * A178912  Number of generalized mountain numbers (see A134853) with n
 *          digits.
 * https://oeis.org/A178912
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, fini, full
 * Formula:   a(n) = number of n‑digit generalized mountain numbers. * * For n
 *            = 1: 9 (all single digits) * For n = 2: 0 (no peak can be
 *            reached) * For 3 ≤ n ≤ 18: * a(n) = Σ_{p=1}^{9}
 *            Σ_{m=max(2,n-p)}^{min(p,n-1)} * binom(p-1, m-1) * binom(p, n-m);
 *            * * For n ≥ 19: 0 (no valid sequences exist)
 * Range:     u8 n <= 3, u16 n <= 18, u32 n <= 18, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* below offset – undefined */
    if (n == 1) return 9;
    if (n >= 19) return 0;         /* no 19‑digit or longer generalized mountains */

    seq_t total = 0;
    for (idx_t p = 1; p <= 9; ++p)
    {
        /* Determine the range of peak positions m that satisfy
           2 ≤ m ≤ n-1, m ≤ p, and n - m ≤ p. */
        idx_t lower = (n > p) ? (n - p) : 0;  /* n - p when positive, else 0 */
        if (lower < 2) lower = 2;
        idx_t upper = (p < n - 1) ? p : (n - 1);
        if (lower > upper) continue;

        for (idx_t m = lower; m <= upper; ++m)
        {
            seq_t inc = binom(p - 1, m - 1);   /* increasing side */
            seq_t dec = binom(p, n - m);       /* decreasing side */
            total += inc * dec;
        }
    }
    return total;
}

/*
 * A048017  Number of nonempty subsets of {1,2,...,n} in which exactly 1/3 of
 *          the elements are <= (n-2)/3.
 * https://oeis.org/A048017
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{m=1}^{⌊n/3⌋} C(⌊(n-2)/3⌋, m) · C(n-⌊(n-2)/3⌋, 2m)
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  (subsets of {1..n} with exactly one third of the elements ≤ (n‑2)/3) */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle small n explicitly (loop works for all n ≥ 1, but this keeps
       L safe for n < 2). */
    idx_t L = (n >= 2) ? ((n - 2) / 3) : 0;   /* floor((n-2)/3) */
    idx_t R = n - L;                           /* size of the upper part */
    idx_t maxM = n / 3;                        /* upper bound for m */

    seq_t result  = 0;
    seq_t combL   = 1;   /* C(L, 0) */
    seq_t combU   = 1;   /* C(R, 0) */
    idx_t t       = 0;   /* current t in combU = C(R, t) */

    for (idx_t m = 1; m <= maxM; ++m) {
        /* Update combL = C(L, m) from C(L, m-1) */
        combL = combL * ((seq_t)(L - m + 1));
        combL = combL / ((seq_t)m);

        /* Update combU = C(R, 2m) from C(R, t) where t = 2*(m-1) */
        combU = combU * ((seq_t)(R - t));
        combU = combU / ((seq_t)(t + 1));
        combU = combU * ((seq_t)(R - t - 1));
        combU = combU / ((seq_t)(t + 2));

        t += 2;   /* Now t = 2m for next iteration */

        /* Add the contribution for this m */
        result += combL * combU;
    }

    return result;
}

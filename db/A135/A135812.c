/*
 * A135812  Number of coincidence-free length n lists of 6-tuples with all
 *          numbers 1..n in tuple position k, for k=1..6.
 * https://oeis.org/A135812
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{j=0..n} (-1)^(n-j) * binomial(n,j) * (j!)^6
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* Computes the OEIS sequence A135812 for all n ≥ 0.  No dynamic
   memory allocation, safe for arbitrary precision seq_t or unsigned
   integer types (8/16/32/64/128 bits). */

seq_t a(idx_t n)
{
    seq_t pos = (seq_t)0;   /* Sum of terms with positive sign   */
    seq_t neg = (seq_t)0;   /* Sum of terms with negative sign   */

    seq_t binom = (seq_t)1;   /* binomial(n,0) */
    seq_t fac_j = (seq_t)1;   /* 0! = 1          */
    /* fac_j will be computed as j! for the current j */

    for (idx_t j = 0; j <= n; ++j) {
        /* Compute (j!)^6 */
        seq_t powj6 = fac_j;
        for (int k = 1; k < 6; ++k) {
            powj6 *= fac_j;
        }

        /* term = binomial(n,j) * (j!)^6 */
        seq_t term = binom * powj6;

        /* Add with appropriate sign */
        if (((n - j) & 1ULL) == 0) {
            pos += term;
        } else {
            neg += term;
        }

        /* Prepare for next j (unless this was the last iteration) */
        if (j < n) {
            /* Update j! → (j+1)! */
            fac_j *= (seq_t)(j + 1);

            /* Update binomial: binomial(n,j+1) = binomial(n,j) * (n-j)/(j+1) */
            binom = binom * (seq_t)(n - j) / (seq_t)(j + 1);
        }
    }

    /* pos ≥ neg for all n; result is non‑negative. */
    return pos - neg;
}

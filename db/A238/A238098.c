/*
 * A238098  Number of cubic polynomials with coefficients from {1..n} for
 *          which all three roots are integers.
 * https://oeis.org/A238098
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{a1≤a2≤a3, a1a2a3≤n} ⌊ n / max(a1+a2+a3,
 *            a1a2+a1a3+a2a3, a1a2a3) ⌋ This counts monic cubic polynomials
 *            with integer roots whose coefficients lie in {1,…,n}, then
 *            multiplies by the number of possible leading coefficients
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Ensure n is at least 1; for n==0 we return 0,
       though the sequence is defined for n>=1. */
    if (n == 0) return 0;

    seq_t result = 0;

    /* Enumerate unordered positive integer triples (a1,a2,a3)
       with product ≤ n.  For each triple we compute the three
       symmetric sums, take their maximum, and add ⌊n/M⌋ to the result. */
    for (idx_t a1 = 1; a1 <= n; ++a1) {
        idx_t limit_a2 = n / a1;          /* a2 can be at most n/a1         */
        for (idx_t a2 = a1; a2 <= limit_a2; ++a2) {
            idx_t prod12 = a1 * a2;       /* a1*a2 fits into idx_t (≤ n²) */
            idx_t limit_a3 = n / prod12;  /* a3 is bounded by n/(a1*a2)   */
            if (limit_a3 < a2) continue;  /* no a3 satisfies a3 ≥ a2       */
            for (idx_t a3 = a2; a3 <= limit_a3; ++a3) {
                seq_t b = (seq_t)a1 + a2 + a3;                  /* sum   */
                seq_t c = (seq_t)a1 * a2 + (seq_t)a1 * a3 + (seq_t)a2 * a3; /* pairwise sum */
                seq_t d = (seq_t)a1 * a2 * a3;                 /* product */
                seq_t M = b;
                if (c > M) M = c;
                if (d > M) M = d;
                if (M > (seq_t)n) continue;
                result += (seq_t)n / M;
            }
        }
    }

    return result;
}

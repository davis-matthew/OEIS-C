/*
 * A054328  Tenth unsigned column of Lanczos triangle A053125 (decreasing
 *          powers).
 * https://oeis.org/A054328
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^n * binomial(2*n+10, 9)
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 6, u64 n <= 17, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for OEIS sequence A054328 */
seq_t a(idx_t n)
{
    /* Compute binomial(2*n+10, 9) */
    idx_t top = 2*n + 10;         /* upper argument of the binomial coefficient */
    seq_t binom = 1;              /* running result for the binomial */

    for (idx_t i = 1; i <= 9; ++i) {
        /* multiply by (top - 9 + i) and divide by i; the division is exact */
        seq_t numer = (seq_t)(top - 9 + i);
        binom = binom * numer / (seq_t)i;
    }

    /* Compute 4^n by repeated multiplication */
    seq_t pow4 = 1;
    for (idx_t i = 0; i < n; ++i) {
        pow4 *= 4;
    }

    /* Final result */
    return pow4 * binom;
}

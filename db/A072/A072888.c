/*
 * A072888  Sum of the coefficients of the Schur function expansion of the
 *          square of the Vandermonde determinant in n variables.
 * https://oeis.org/A072888
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  hard, sign
 * Formula:   a(n) = ∏_{x=0..⌊n/2⌋} (1‑3x) * ∏_{x=0..⌊(n‑1)/2⌋} (1+6x)
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 10, i64 n <= 10, i128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n < 2 the OEIS offset is 2; behaviour is undefined.  */
    if (n < 2) return 0;

    seq_t result = 1;
    idx_t m = n / 2;                /* floor(n/2) */
    for (idx_t x = 0; x <= m; ++x) {
        seq_t factor = 1 - (seq_t)3 * (seq_t)x;
        result *= factor;
    }

    idx_t k = (n - 1) / 2;           /* floor((n‑1)/2) */
    for (idx_t x = 0; x <= k; ++x) {
        seq_t factor = 1 + (seq_t)6 * (seq_t)x;
        result *= factor;
    }

    return result;
}

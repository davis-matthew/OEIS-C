/*
 * A160141  Numerator of Hermite(n, 14/27).
 * https://oeis.org/A160141
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 27^n * Hermite(n, 14/27)
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 10, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* A closed-form recurrence for the scaled Hermite values:
       S_0 = 1, S_1 = 28,
       S_{k+1} = 28 * S_k - 1458 * k * S_{k-1}   (k >= 1)
       where S_k = 27^k * Hermite(k, 14/27). */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)28;

    seq_t prev2 = (seq_t)1;   /* S_0 */
    seq_t prev1 = (seq_t)28;  /* S_1 */
    seq_t cur = 0;

    for (idx_t i = 2; i <= n; ++i) {
        /* coefficient = 1458 * (i-1) = 2*(i-1)*729 */
        seq_t coeff = (seq_t)1458 * (seq_t)(i - 1);
        cur = (seq_t)28 * prev1 - coeff * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}

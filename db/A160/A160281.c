/*
 * A160281  Numerator of Hermite(n, 19/29).
 * https://oeis.org/A160281
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 29^n * Hermite(n, 19/29)
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 9, i128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1, a(1) = 38                              */
/*             a(n) = 38 * a(n-1) - 1682 * (n-1) * a(n-2)      */
/*             for n >= 2                                        */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)38;

    seq_t prev2 = (seq_t)1;          /* a(0) */
    seq_t prev1 = (seq_t)38;         /* a(1) */
    seq_t curr;

    for (idx_t k = 2; k <= n; ++k) {
        /* curr = 38*prev1 - 1682*(k-1)*prev2 */
        seq_t coeff = (seq_t)1682 * (seq_t)(k - 1);
        curr = (seq_t)38 * prev1 - coeff * prev2;

        /* shift window */
        prev2 = prev1;
        prev1 = curr;
    }

    return curr;
}

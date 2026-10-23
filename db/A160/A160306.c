/*
 * A160306  Numerator of Hermite(n, 8/31).
 * https://oeis.org/A160306
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(0) = 1, a(1) = 16; a(n+2) = 16*a(n+1) - 1922*(n+1)*a(n) for n
 *            ≥ 0
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 9, i128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Handle the offset */
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)16;

    /* Iterate using two previous values */
    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)16;  /* a(1) */
    /* Number of iterations: n-1 = (n-2) + 1 */
    idx_t limit = n - 2;          /* n ≥ 2 here, so no underflow */

    for (idx_t i = 0; i <= limit; ++i) {
        /* i corresponds to the n in a(n+2) = … where n = i + 1 in the
           recurrence term (n+1) * a(n) with a(n) being prev2. */
        seq_t mult_factor = (seq_t)(i + 1);
        seq_t term = ((seq_t)1922) * mult_factor * prev2;
        seq_t next = ((seq_t)16) * prev1 - term;
        /* Shift for next iteration */
        prev2 = prev1;
        prev1 = next;
    }
    return prev1;
}

/*
 * A160005  Numerator of Hermite(n, 3/25).
 * https://oeis.org/A160005
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 25^n * Hermite(n, 3/25) Recurrence: a(0) = 1, a(1) = 6,
 *            a(n+1) = 6*a(n) - 1250*n*a(n-1) (n ≥ 1)
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 5, i64 n <= 10, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)6;

    seq_t prev = (seq_t)1;   /* a(0) */
    seq_t curr = (seq_t)6;   /* a(1) */

    for (idx_t k = 1; k < n; ++k)   /* compute a(k+1) from a(k) and a(k-1) */
    {
        seq_t next = (seq_t)6 * curr - ((seq_t)1250 * (seq_t)k) * prev;
        prev = curr;
        curr = next;
    }
    return curr;   /* a(n) */
}

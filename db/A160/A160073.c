/*
 * A160073  Numerator of Hermite(n, 9/26).
 * https://oeis.org/A160073
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 9*a(n-1) - 338*(n-1)*a(n-2), a(0)=1, a(1)=9
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 6, i64 n <= 12, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)9;

    seq_t prevprev = (seq_t)1;   /* a(0) */
    seq_t prev = (seq_t)9;       /* a(1) */
    seq_t curr;

    for (idx_t i = 2; i <= n; ++i) {
        curr = 9 * prev - 338 * ((seq_t)(i - 1)) * prevprev;
        prevprev = prev;
        prev = curr;
    }
    return prev;
}

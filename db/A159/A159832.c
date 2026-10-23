/*
 * A159832  Numerator of Hermite(n, 13/22).
 * https://oeis.org/A159832
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 13*a(n-1) - 242*(n-1)*a(n-2) (n >= 2), a(0) = 1, a(1) =
 *            13
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 6, i64 n <= 12, i128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;   /* a(0) = 1 */
    if (n == 1)
        return 13;  /* a(1) = 13 */

    seq_t prevPrev = 1;  /* a(0) */
    seq_t prev     = 13; /* a(1) */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i)
    {
        /* a(i) = 13*a(i-1) - 242*(i-1)*a(i-2) */
        cur = 13 * prev - ((seq_t)242 * (seq_t)(i - 1)) * prevPrev;
        prevPrev = prev;
        prev = cur;
    }
    return prev;
}

/*
 * A160292  Numerator of Hermite(n, 7/30).
 * https://oeis.org/A160292
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   not stated
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 6, i64 n <= 11, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 15^n * Hermite(n, 7/30), which satisfies
 *   a(0) = 1
 *   a(1) = 7
 *   a(n+1) = 7*a(n) - 450*n*a(n-1)   (n >= 1)
 * The recurrence follows from the physicists Hermite polynomial
 * recurrence H_{n+1}(x) = 2x H_n(x) - 2n H_{n-1}(x) with x = 7/30
 * and the scaling factor 15^n.
 */
seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 7;

    seq_t prevprev = 1; /* a(0) */
    seq_t prev     = 7; /* a(1) */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t factor = (seq_t)450 * (seq_t)(i - 1);     /* 450*(i-1) */
        seq_t curr   = 7 * prev - factor * prevprev;    /* a(i) */
        prevprev = prev;
        prev     = curr;
    }
    return prev;
}

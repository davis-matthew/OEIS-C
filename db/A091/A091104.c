/*
 * A091104  Expansion of (1-4x+24x^2)/((1-4x)(1+4x)).
 * https://oeis.org/A091104
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = (3*4^n + 7*(-4)^n)/4 for n > 0, a(0) = 1
 * Range:     i8 n <= 1, i16 n <= 5, i32 n <= 13, i64 n <= 23, i128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS A091104.  seq_t and idx_t are supplied by
 * the build system.  The implementation uses only integer arithmetic and
 * works for any integer type (fixed‑width or arbitrary precision).
 */
seq_t a(idx_t n)
{
    /* Handle the offset explicitly. */
    if (n == 0)
        return (seq_t)1;

    /* Compute 4^n iteratively (fast enough for n up to about 10 000). */
    seq_t pow4 = (seq_t)1;
    for (idx_t i = 0; i < n; ++i)
        pow4 *= (seq_t)4;

    /* (-4)^n = (-1)^n * 4^n.  Use the sign of n to apply the minus. */
    seq_t pow_neg4 = (n & 1) ? -pow4 : pow4;

    /* a(n) = (3*4^n + 7*(-4)^n) / 4. */
    seq_t numerator = (seq_t)3 * pow4 + (seq_t)7 * pow_neg4;
    return numerator / (seq_t)4;
}

/*
 * A078044  Expansion of (1-x)/(1+x+2*x^3).
 * https://oeis.org/A078044
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -a(n-1) - 2*a(n-3); a(0) = 1, a(1) = -2, a(2) = 2
 * Range:     i8 n <= 9, i16 n <= 19, i32 n <= 40, i64 n <= 82, i128 n <= 166,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)-2;
    if (n == 2u) return (seq_t)2;

    seq_t v0 = (seq_t)1;   /* a(n-3) */
    seq_t v1 = (seq_t)-2;  /* a(n-2) */
    seq_t v2 = (seq_t)2;   /* a(n-1) */

    for (idx_t i = 3; i <= n; ++i)
    {
        seq_t next = -v2 - (seq_t)2 * v0; /* a(n) = -a(n-1) - 2*a(n-3) */
        v0 = v1;
        v1 = v2;
        v2 = next;
    }
    return v2;
}

/*
 * A147974  a(n) = n^3-((n-1)^3+(n-2)^3+(n-3)^3).
 * https://oeis.org/A147974
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -2*n^3 + 18*n^2 - 42*n + 36
 * Range:     i8 n <= 7, i16 n <= 28, i32 n <= 1027, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x  = (seq_t)n;      /* n */
    seq_t n2 = x * x;         /* n^2 */
    seq_t n3 = n2 * x;        /* n^3 */
    return -2 * n3 + 18 * n2 - 42 * x + 36;
}

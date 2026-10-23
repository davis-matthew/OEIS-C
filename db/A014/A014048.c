/*
 * A014048  Inverse of 39th cyclotomic polynomial.
 * https://oeis.org/A014048
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (n/39 + 1) * - (n >= 3 ? ((n-3)/39 + 1) : 0) * - (n >= 13
 *            ? ((n-13)/39 + 1) : 0) * + (n >= 16 ? ((n-16)/39 + 1) : 0)
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 0;
    res += (seq_t)(n / 39 + 1);
    if (n >= 3)  res -= (seq_t)((n - 3) / 39 + 1);
    if (n >= 13) res -= (seq_t)((n - 13) / 39 + 1);
    if (n >= 16) res += (seq_t)((n - 16) / 39 + 1);
    return res;
}

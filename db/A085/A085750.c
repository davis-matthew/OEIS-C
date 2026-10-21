/*
 * A085750  Determinant of the symmetric n X n matrix A defined by A[i,j] =
 *          |i-j| for 1 <= i,j <= n.
 * https://oeis.org/A085750
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(1) = 0 for n ≥ 2: a(n) = (-1)^(n+1) * (n-1) * 2^(n-2) (O(n)
 *            time)
 * Range:     i8 n <= 6, i16 n <= 13, i32 n <= 28, i64 n <= 59, i128 n <= 122,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1U)                 /* a(1) = 0; a(0) not defined but safe */
        return (seq_t)0;

    /* sign = (-1)^(n+1): positive when n+1 is even (i.e., n odd) */
    seq_t sign = ((n + 1U) & 1U) ? (seq_t)-1 : (seq_t)1;

    /* compute 2^(n-2) by repeated multiplication */
    seq_t pow2 = (seq_t)1;
    for (idx_t i = 0; i < n - 2U; ++i)
        pow2 *= (seq_t)2;

    /* multiply by (n-1) */
    seq_t term = (seq_t)(n - 1U) * pow2;

    return sign * term;
}

/*
 * A135997  Table of triples T(k,m) = k (m=1), 2-k (m=2) and 1-k (m=3).
 * https://oeis.org/A135997
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabf, less, easy
 * Formula:   a(n) = if n mod 3 = 0 then n/3, else if n mod 3 = 1 then 2 -
 *            n/3, else 1 - n/3 The sequence enumerates the triples (k, 2-k,
 *            1-k) row-wise
 * Range:     i8 n <= 383, i16 n <= 9002, i32 n <= 9002, i64 n <= 9002,
 *            i128 n <= 9002, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t r = n / 3;          /* row index k */
    idx_t c = n % 3;          /* column index 0,1,2 */
    seq_t sr = (seq_t)r;      /* cast row to result type */

    if (c == 0)                return sr;
    if (c == 1)                return (seq_t)2 - sr;
    return (seq_t)1 - sr;
}

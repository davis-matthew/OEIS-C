/*
 * A064322  Triply triangular numbers.
 * https://oeis.org/A064322
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A000217(A000217(A000217(n))) = n*(n+1)/2 applied three
 *            times
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 26, u64 n <= 400, u128 n <= 400,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t t1 = (nn * (nn + 1)) / 2;          /* first triangular */
    seq_t t2 = (t1 * (t1 + 1)) / 2;          /* second triangular */
    seq_t res = (t2 * (t2 + 1)) / 2;         /* third triangular */
    return res;
}

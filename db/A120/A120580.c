/*
 * A120580  Hankel transform of Sum_{k=0..n} C(2k,k).
 * https://oeis.org/A120580
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 2*a(n-1) - 4*a(n-2), a(0)=1, a(1)=0
 * Range:     i8 n <= 7, i16 n <= 16, i32 n <= 31, i64 n <= 40, i128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the n-th term of OEIS A120580 (Hankel transform of Sum_{k=0..n} C(2k,k)). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;

    seq_t prevprev = (seq_t)1;   /* a(0) */
    seq_t prev     = (seq_t)0;   /* a(1) */
    seq_t curr;

    for (idx_t i = 2; i <= n; ++i) {
        curr = (seq_t)2 * prev - (seq_t)4 * prevprev;
        prevprev = prev;
        prev     = curr;
    }
    return curr;
}

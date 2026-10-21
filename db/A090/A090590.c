/*
 * A090590  (1,1) entry of powers of the orthogonal design shown below.
 * https://oeis.org/A090590
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2*a(n-1) - 8*a(n-2); a(1)=1, a(2)=-6
 * Range:     i8 n <= 4, i16 n <= 10, i32 n <= 20, i64 n <= 43, i128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)-6;
    seq_t a_prev2 = (seq_t)1;   /* a(1) */
    seq_t a_prev1 = (seq_t)-6;  /* a(2) */
    seq_t next;
    for (idx_t i = 3; i <= n; ++i) {
        next = (seq_t)2 * a_prev1 - (seq_t)8 * a_prev2;
        a_prev2 = a_prev1;
        a_prev1 = next;
    }
    return a_prev1;
}

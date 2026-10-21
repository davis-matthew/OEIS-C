/*
 * A050935  a(n) = a(n-1) - a(n-3) with a(1)=0, a(2)=0, a(3)=1.
 * https://oeis.org/A050935
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nice, sign
 * Formula:   a(n) = a(n-1) - a(n-3) with a(1)=0, a(2)=0, a(3)=1
 * Range:     i8 n <= 38, i16 n <= 78, i32 n <= 158, i64 n <= 315,
 *            i128 n <= 631, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)0;
    if (n == 3) return (seq_t)1;

    /* iteratively compute the recurrence from 4 to n */
    seq_t a1 = (seq_t)0; /* a(n-3) */
    seq_t a2 = (seq_t)0; /* a(n-2) */
    seq_t a3 = (seq_t)1; /* a(n-1) */

    for (idx_t i = 4; i <= n; ++i) {
        seq_t new_val = a3 - a1; /* a(i) = a(i-1) - a(i-3) */
        a1 = a2;
        a2 = a3;
        a3 = new_val;
    }
    return a3;
}

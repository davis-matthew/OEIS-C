/*
 * A194527  Second coordinate of (5,6)-Lagrange pair for n.
 * https://oeis.org/A194527
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = n + 1 + floor(2*n/11) - 5*floor((2*n + 5)/11) -
 *            floor((2*n + 10)/11)
 * Range:     i8 n <= 100, i16 n <= 100, i32 n <= 100, i64 n <= 100,
 *            i128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is positive; return the value for a(1) at n==1. */
    if (n == 0)
        return 0;   /* offset is 1, but safe for out-of-range */
    unsigned __int128 nn = (unsigned __int128)n;

    seq_t t1 = (seq_t)(nn + 1);                       /* n + 1 */
    seq_t t2 = (seq_t)((2 * nn) / 11);                /* floor(2*n/11) */
    seq_t t3 = (seq_t)(5 * ((2 * nn + 5) / 11));      /* 5*floor((2*n + 5)/11) */
    seq_t t4 = (seq_t)((2 * nn + 10) / 11);           /* floor((2*n + 10)/11) */

    return t1 + t2 - t3 - t4;
}

/*
 * A214779  a(n) = 3*a(n-2) - a(n-3) with a(0)=-1, a(1)=1, a(2)=-4.
 * https://oeis.org/A214779
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 3*a(n-2) - a(n-3) initial values: a(0) = -1, a(1) = 1,
 *            a(2) = -4
 * Range:     i8 n <= 7, i16 n <= 16, i32 n <= 33, i64 n <= 33, i128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Direct cases for the first three indices */
    if (n == 0) return (seq_t)(-1);
    if (n == 1) return (seq_t)(1);
    if (n == 2) return (seq_t)(-4);

    /* Rolling variables: a0 = a(i-3), a1 = a(i-2), a2 = a(i-1) */
    seq_t a0 = (seq_t)(-1);   /* a(0) */
    seq_t a1 = (seq_t)(1);    /* a(1) */
    seq_t a2 = (seq_t)(-4);   /* a(2) */

    /* Iterate from i = 3 up to n, computing a(i) = 3*a1 - a0 */
    for (idx_t i = 3; i <= n; ++i) {
        seq_t next = (seq_t)3 * a1 - a0;
        /* Shift the window: a0 <- a1, a1 <- a2, a2 <- next */
        a0 = a1;
        a1 = a2;
        a2 = next;
    }

    /* After the loop, a2 holds a(n) */
    return a2;
}

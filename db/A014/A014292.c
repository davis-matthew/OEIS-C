/*
 * A014292  a(n) = 2*a(n-1) - a(n-2) - a(n-4) with a(0) = a(1) = 0, a(2) = 1,
 *          a(3) = 2.
 * https://oeis.org/A014292
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-2) - a(n-4) with a(0)=0, a(1)=0, a(2)=1,
 *            a(3)=2
 * Range:     i8 n <= 15, i16 n <= 30, i32 n <= 59, i64 n <= 121,
 *            i128 n <= 241, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle trivial cases directly */
    if (n == 0) return 0;
    if (n == 1) return 0;
    if (n == 2) return 1;
    if (n == 3) return 2;

    /* initialise the first four values */
    seq_t v0 = 0;   /* a(0) */
    seq_t v1 = 0;   /* a(1) */
    seq_t v2 = 1;   /* a(2) */
    seq_t v3 = 2;   /* a(3) */
    seq_t next;

    /* iterate from 4 up to n */
    for (idx_t i = 4; i <= n; ++i) {
        next = 2 * v3 - v2 - v0;   /* a(i) = 2*a(i-1) - a(i-2) - a(i-4) */
        v0 = v1;
        v1 = v2;
        v2 = v3;
        v3 = next;
    }
    return v3;
}

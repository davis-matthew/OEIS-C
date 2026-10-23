/*
 * A134314  First differences of A134429.
 * https://oeis.org/A134314
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, less
 * Formula:   a(n) = -2*a(n-1)-2*a(n-2)-2*a(n-3)-a(n-4); Initial values: a(0)
 *            = -8, a(1) = 8, a(2) = -8, a(3) = 16
 * Range:     i8 n <= 30, i16 n <= 51, i32 n <= 51, i64 n <= 51, i128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first four terms directly */
    if (n == 0) return -8;
    if (n == 1) return  8;
    if (n == 2) return -8;
    if (n == 3) return 16;

    /* Use a rolling window of the last four values */
    seq_t a0 = -8;   /* a(n-4) */
    seq_t a1 =  8;   /* a(n-3) */
    seq_t a2 = -8;   /* a(n-2) */
    seq_t a3 = 16;   /* a(n-1) */

    for (idx_t i = 4; i <= n; ++i) {
        seq_t next = ((seq_t)-2) * a3
                   + ((seq_t)-2) * a2
                   + ((seq_t)-2) * a1
                   + ((seq_t)-1) * a0;

        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }

    return a3;
}

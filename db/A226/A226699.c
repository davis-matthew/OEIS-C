/*
 * A226699  Solutions x of the Pell equation x^2 - 61*y^2 = +4.
 * https://oeis.org/A226699
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1523*a(n-1) - a(n-2), a(0)=2, a(1)=1523
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 6, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first two terms directly */
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)1523;

    /* Iteratively compute the recurrence up to n */
    seq_t a0 = (seq_t)2;       /* a(0) */
    seq_t a1 = (seq_t)1523;    /* a(1) */
    seq_t a2;
    for (idx_t i = 2; i <= n; ++i) {
        a2 = (seq_t)1523 * a1 - a0;
        a0 = a1;
        a1 = a2;
    }
    return a1; /* a(n) */
}

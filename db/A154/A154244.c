/*
 * A154244  a(n) = 6*a(n-1) - 2*a(n-2) for n>1; a(1)=1, a(2)=6.
 * https://oeis.org/A154244
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - 2*a(n-2), a(1)=1, a(2)=6
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 26, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle initial terms directly. */
    if (n == 1)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)6;

    /* Iteratively compute the recurrence up to n. */
    seq_t a1 = (seq_t)1;   /* a(1) */
    seq_t a2 = (seq_t)6;   /* a(2) */
    idx_t i;

    for (i = 3; i <= n; ++i) {
        seq_t a3 = (seq_t)(6 * a2 - 2 * a1); /* a(i) = 6*a(i-1) - 2*a(i-2) */
        a1 = a2;
        a2 = a3;
    }
    return a2;
}

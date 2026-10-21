/*
 * A205249  Number of (n+1) X 3 0..1 arrays with the number of clockwise edge
 *          increases in every 2 X 2 subblock the same.
 * https://oeis.org/A205249
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) - 7*a(n-2) + 2*a(n-3); Initial values: a(1) =
 *            40, a(2) = 168, a(3) = 752
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 13, u64 n <= 27, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first three terms directly. */
    if (n == 0) return (seq_t)0;          /* out of bounds, not used */
    if (n == 1) return (seq_t)40;
    if (n == 2) return (seq_t)168;
    if (n == 3) return (seq_t)752;

    /* a1 = a(i-3), a2 = a(i-2), a3 = a(i-1) */
    seq_t a1 = (seq_t)40;
    seq_t a2 = (seq_t)168;
    seq_t a3 = (seq_t)752;
    seq_t an = a3; /* placeholder */

    for (idx_t i = 4; i <= n; ++i) {
        an = (seq_t)6 * a3 - (seq_t)7 * a2 + (seq_t)2 * a1;
        /* shift the window forward */
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return an;
}

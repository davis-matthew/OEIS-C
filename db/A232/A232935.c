/*
 * A232935  Number of n X 2 0..2 arrays with no element x(i,j) adjacent to
 *          value 2-x(i,j) horizontally, vertically or antidiagonally.
 * https://oeis.org/A232935
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 2*a(n-2) + a(n-3) for n ≥ 4 Initial values:
 *            a(1)=6, a(2)=12, a(3)=26
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 27, u64 n <= 56, u128 n <= 114,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    /* handle the initial values */
    if (n == 1) return (seq_t)6;
    if (n == 2) return (seq_t)12;
    if (n == 3) return (seq_t)26;

    /* use only the last three terms */
    seq_t a3 = (seq_t)6;   /* a(n-3)  */
    seq_t a2 = (seq_t)12;  /* a(n-2)  */
    seq_t a1 = (seq_t)26;  /* a(n-1)  */
    seq_t cur;

    for (idx_t i = 4; i <= n; ++i) {
        cur = a3 + (seq_t)2 * a2 + a1;
        a3 = a2;
        a2 = a1;
        a1 = cur;
    }
    return a1;
}

/*
 * A171997  a(n) = a(n-1) + a(n-2) - floor(a(n-2)/2) - floor(a(n-5)/2);
 *          initial terms are 1, 1, 2, 3, 4.
 * https://oeis.org/A171997
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-2) - floor(a(n-2)/2) - floor(a(n-5)/2),
 *            a(1)=1, a(2)=1, a(3)=2, a(4)=3, a(5)=4
 * Range:     u8 n <= 26, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)2;
    if (n == 4) return (seq_t)3;
    if (n == 5) return (seq_t)4;

    /* Sliding window for last five terms */
    seq_t a1 = (seq_t)1; /* a(n-5) at start of iteration */
    seq_t a2 = (seq_t)1; /* a(n-4)   */
    seq_t a3 = (seq_t)2; /* a(n-3)   */
    seq_t a4 = (seq_t)3; /* a(n-2)   */
    seq_t a5 = (seq_t)4; /* a(n-1)   */

    seq_t an = 0;
    for (idx_t i = 6; i <= n; ++i) {
        an = a5 + a4 - a4 / (seq_t)2 - a1 / (seq_t)2;

        /* Advance window for next iteration */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = an;
    }

    return an;
}

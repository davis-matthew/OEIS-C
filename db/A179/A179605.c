/*
 * A179605  Eight white kings and one red king on a 3 X 3 chessboard. G.f.: (1
 *          + 3*x - 2*x^2)/(1 - 2*x - 9*x^2 - 2*x^3).
 * https://oeis.org/A179605
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*a(n-1) + 9*a(n-2) + 2*a(n-3) Initial values: a(0) = 1,
 *            a(1) = 5, a(2) = 17
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first three indices directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;
    if (n == 2) return (seq_t)17;

    /* Iterate from 3 up to n, updating the last three terms */
    seq_t a0 = (seq_t)1;   /* a(n-3) */
    seq_t a1 = (seq_t)5;   /* a(n-2) */
    seq_t a2 = (seq_t)17;  /* a(n-1) */
    seq_t curr;

    for (idx_t i = 3; i <= n; ++i) {
        curr = (seq_t)(2 * a2 + 9 * a1 + 2 * a0);
        /* shift for next iteration */
        a0 = a1;
        a1 = a2;
        a2 = curr;
    }
    return curr;
}

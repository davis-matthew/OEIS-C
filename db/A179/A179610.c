/*
 * A179610  Eight white kings and one red king on a 3 X 3 chessboard. G.f.:
 *          1/(1-3*x-5*x^2+4*x^3).
 * https://oeis.org/A179610
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*a(n-1)+5*a(n-2)-4*a(n-3) initial values: a(0)=1,
 *            a(1)=3, a(2)=14
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first three values directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)14;

    /* Iteratively compute up to a(n) keeping only the last three terms. */
    seq_t a0 = 1;   /* a(n-3)  */
    seq_t a1 = 3;   /* a(n-2)  */
    seq_t a2 = 14;  /* a(n-1)  */
    seq_t a_next;

    /* Use idx_t for the loop counter to match the build's integer type. */
    for (idx_t i = 3; i <= n; ++i) {
        a_next = 3 * a2 + 5 * a1 - 4 * a0;
        a0 = a1;
        a1 = a2;
        a2 = a_next;
    }

    return a2;
}

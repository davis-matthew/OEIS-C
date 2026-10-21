/*
 * A092936  Area of n-th triple of hexagons around a triangle.
 * https://oeis.org/A092936
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 10*(a(n-1)+a(n-2)) - a(n-3), a(1)=1, a(2)=9, a(3)=100
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the first three terms directly */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)9;
    if (n == 3) return (seq_t)100;

    /* iterative computation of the linear recurrence */
    seq_t a1 = (seq_t)1;   /* a(n-3)  */
    seq_t a2 = (seq_t)9;   /* a(n-2)  */
    seq_t a3 = (seq_t)100; /* a(n-1)  */
    seq_t an;

    for (idx_t i = 4; i <= n; ++i) {
        an = 10 * (a3 + a2) - a1;  /* a(i) */
        a1 = a2;   /* shift window */
        a2 = a3;
        a3 = an;
    }
    return a3;
}

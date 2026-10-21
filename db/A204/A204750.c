/*
 * A204750  Number of (n+2) X 6 0..1 arrays with every 3 X 3 subblock having
 *          three equal elements in a row horizontally, vertically, diagonally
 *          or antidiagonally exactly three ways, and new values 0..1
 *          introduced in row major order.
 * https://oeis.org/A204750
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-3) for n > 5; a(1)=188, a(2)=28, a(3)=27,
 *            a(4)=32, a(5)=40
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 44, u64 n <= 90, u128 n <= 183,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    if (n == 1) return (seq_t)188;
    if (n == 2) return (seq_t)28;
    if (n == 3) return (seq_t)27;
    if (n == 4) return (seq_t)32;
    if (n == 5) return (seq_t)40;

    seq_t t0 = (seq_t)27;   /* a(3)  */
    seq_t t1 = (seq_t)32;   /* a(4)  */
    seq_t t2 = (seq_t)40;   /* a(5)  */

    for (idx_t i = 6; i <= n; ++i) {
        seq_t ai = (seq_t)2 * t2 - t0;
        t0 = t1;
        t1 = t2;
        t2 = ai;
    }
    return t2;
}

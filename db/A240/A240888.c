/*
 * A240888  Number of n X 3 0..2 arrays with no element equal to a different
 *          number of vertical neighbors than horizontal neighbors, with new
 *          values 0..2 introduced in row major order.
 * https://oeis.org/A240888
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 8*a(n-2) + 14*a(n-3) + 4*a(n-4) Initial
 *            values: a(1)=2, a(2)=11, a(3)=57, a(4)=289
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 27, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)11;
    if (n == 3) return (seq_t)57;
    if (n == 4) return (seq_t)289;

    seq_t a1 = (seq_t)2;
    seq_t a2 = (seq_t)11;
    seq_t a3 = (seq_t)57;
    seq_t a4 = (seq_t)289;
    seq_t next = 0;

    for (idx_t i = 5; i <= n; ++i) {
        next = 3 * a4 + 8 * a3 + 14 * a2 + 4 * a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = next;
    }
    return a4;
}

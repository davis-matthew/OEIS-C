/*
 * A205313  Number of (n+1) X 4 0..1 arrays with every 2 X 2 subblock having
 *          the same number of equal edges, and new values 0..1 introduced in
 *          row major order.
 * https://oeis.org/A205313
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*a(n-1) - 24*a(n-2) + 21*a(n-3) - 6*a(n-4) with
 *            a(1)=56, a(2)=376, a(3)=2606, a(4)=18152
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t init[4] = {56, 376, 2606, 18152};
    if (n <= 4) {
        return init[n - 1];
    }
    seq_t a1 = init[0], a2 = init[1], a3 = init[2], a4 = init[3];
    for (idx_t i = 5; i <= n; ++i) {
        seq_t next = (seq_t)10 * a4 - (seq_t)24 * a3 + (seq_t)21 * a2 - (seq_t)6 * a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = next;
    }
    return a4;
}

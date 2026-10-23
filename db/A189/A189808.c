/*
 * A189808  Number of right triangles on a (n+1) X 4 grid.
 * https://oeis.org/A189808
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 12*n^2 + 88*n - 240 for n > 8 Initial values: a(1)=28,
 *            a(2)=94, a(3)=200, a(4)=342, a(5)=524, a(6)=732, a(7)=972,
 *            a(8)=1236 Recurrence: a(n) = 3*a(n-1) - 3*a(n-2) + a(n-3) for n
 *            > 8
 * Range:     u8 n <= 3, u16 n <= 70, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static const seq_t table[8] = {28, 94, 200, 342, 524, 732, 972, 1236};
    if (n <= 8) {
        return table[n-1];
    }
    seq_t tn = n;                   /* cast index to seq_t */
    return 12 * tn * tn + 88 * tn - 240;
}

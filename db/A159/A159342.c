/*
 * A159342  Transform of the finite sequence (1, 0, -1, 0, 1, 0, -1) by the
 *          T_{0,1} transform (see link).
 * https://oeis.org/A159342
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*a(n-1) - 2*a(n-2) + a(n-3) for n ≥ 9, with initial
 *            values: a(0)=2, a(1)=3, a(2)=6, a(3)=16, a(4)=39, a(5)=89,
 *            a(6)=207, a(7)=480, a(8)=1116
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 25, u64 n <= 52, u128 n <= 104,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* initial 9 values */
    static const seq_t init[9] = {
        (seq_t)2,  (seq_t)3,  (seq_t)6,  (seq_t)16, (seq_t)39,
        (seq_t)89, (seq_t)207, (seq_t)480, (seq_t)1116
    };

    if (n < 9)
        return init[n];

    /* sliding window: x1 = a(k-1), x2 = a(k-2), x3 = a(k-3) */
    seq_t x1 = init[8]; /* a(8) */
    seq_t x2 = init[7]; /* a(7) */
    seq_t x3 = init[6]; /* a(6) */

    for (idx_t k = 9; k <= n; ++k) {
        seq_t next = (3 * x1) - (2 * x2) + x3;
        /* shift the window forward */
        x3 = x2;
        x2 = x1;
        x1 = next;
    }
    return x1;
}

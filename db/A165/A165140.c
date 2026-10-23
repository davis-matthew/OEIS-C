/*
 * A165140  Number of reduced words of length n in Coxeter group on 33
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^8 = I.
 * https://oeis.org/A165140
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(1)=33, a(2)=1056, a(3)=33792, a(4)=1081344,
 *            a(5)=34603008, a(6)=1107296256, a(7)=35433480192. For n ≥ 8:
 *            a(n) = 31*a(n-1) + 31*a(n-2) + 31*a(n-3) + 31*a(n-4) + 31*a(n-5)
 *            + 31*a(n-6) + 31*a(n-7) - 496*a(n-8) + (n==8?1:0)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t init[8] = {
        (seq_t)1,
        (seq_t)33,
        (seq_t)1056,
        (seq_t)33792,
        (seq_t)1081344,
        (seq_t)34603008,
        (seq_t)1107296256,
        (seq_t)35433480192
    };

    if (n < 8)
        return init[(idx_t)n];

    seq_t buf[8];
    for (int i = 0; i < 8; ++i)
        buf[i] = init[i];

    seq_t cur;
    for (idx_t i = 8; i <= n; ++i) {
        cur = 0;
        /* sum 31 * a(n-1) .. a(n-7) */
        for (int j = 1; j < 8; ++j)
            cur += (seq_t)31 * buf[j];
        /* subtract 496 * a(n-8) */
        cur -= (seq_t)496 * buf[0];
        /* add correction for n==8 */
        if (i == 8)
            cur += (seq_t)1;

        /* rotate buffer: discard a(n-8), shift others, store a(n) */
        for (int j = 0; j < 7; ++j)
            buf[j] = buf[j + 1];
        buf[7] = cur;
    }

    return cur;
}

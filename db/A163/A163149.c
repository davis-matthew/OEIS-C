/*
 * A163149  Number of reduced words of length n in Coxeter group on 22
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^4 = I.
 * https://oeis.org/A163149
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = -210*a(n-4) + 20*(a(n-1)+a(n-2)+a(n-3)) valid for n ≥ 5.
 *            Initial values: a(0) = 1, a(1) = 22, a(2) = 462, a(3) = 9702,
 *            a(4) = 203511
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base values for n = 0..4 */
    static const seq_t base[5] = {
        (seq_t)1,
        (seq_t)22,
        (seq_t)462,
        (seq_t)9702,
        (seq_t)203511
    };

    if (n < 5)
        return base[n];

    /* Sliding window: a(n-4), a(n-3), a(n-2), a(n-1) */
    seq_t buf[4];
    buf[0] = base[1]; /* a(1)  */
    buf[1] = base[2]; /* a(2)  */
    buf[2] = base[3]; /* a(3)  */
    buf[3] = base[4]; /* a(4)  */

    /* Iterate from n = 5 up to the requested n */
    for (idx_t i = 5; i <= n; ++i)
    {
        seq_t tri            = buf[1] + buf[2] + buf[3];
        seq_t new_val        = (seq_t)20 * tri;
        new_val -= (seq_t)210 * buf[0]; /* 20*sum - 210*a(n-4) */

        /* Shift window forward */
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = new_val;
    }

    return buf[3]; /* a(n) */
}

/*
 * A129638  Number of meaningful differential operations of the k-th order on
 *          the space R^11.
 * https://oeis.org/A129638
 *
 * Offset:    11    (a(11) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(k+6) = a(k+5)+5*a(k+4) -4*a(k+3) -6*a(k+2) +3*a(k+1) +a(k)
 *            Offset 11: a(11)=11, a(12)=21, a(13)=40, a(14)=77, a(15)=148,
 *            a(16)=286
 * Range:     u8 n <= 15, u16 n <= 24, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* values before the offset are undefined; return 0 */
    if (n < 11) return (seq_t)0;
    static const seq_t init[6] = {(seq_t)11, (seq_t)21, (seq_t)40,
                                  (seq_t)77, (seq_t)148, (seq_t)286};
    if (n <= 16) return init[n - 11];

    seq_t cur_vals[6];
    cur_vals[0] = init[0];
    cur_vals[1] = init[1];
    cur_vals[2] = init[2];
    cur_vals[3] = init[3];
    cur_vals[4] = init[4];
    cur_vals[5] = init[5];

    for (idx_t cur = 17; cur <= n; ++cur) {
        seq_t pos = cur_vals[5] + cur_vals[4] * (seq_t)5
                    + cur_vals[0] + cur_vals[1] * (seq_t)3;
        seq_t neg = cur_vals[3] * (seq_t)4 + cur_vals[2] * (seq_t)6;
        seq_t next = pos - neg;      /* pos >= neg, so no underflow */

        /* slide window of last six values */
        cur_vals[0] = cur_vals[1];
        cur_vals[1] = cur_vals[2];
        cur_vals[2] = cur_vals[3];
        cur_vals[3] = cur_vals[4];
        cur_vals[4] = cur_vals[5];
        cur_vals[5] = next;
    }
    return cur_vals[5];
}

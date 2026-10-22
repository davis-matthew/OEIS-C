/*
 * A002527  Number of permutations p on the set [n] with the properties that
 *          abs(p(i)-i) <= 3 for all i and p(1) <= 3.
 * https://oeis.org/A002527
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 2*a(n-2) + 10*a(n-4) + 8*a(n-5) - 2*a(n-6) -
 *            16*a(n-7) - 10*a(n-8) - 2*a(n-9) + 4*a(n-10) + 2*a(n-11) +
 *            2*a(n-13) + a(n-14) for n >= 14, a(0..13) = 0, 1, 2, 6, 18, 60,
 *            184, 560, 1695, 5200, 15956, 48916, 149664, 458048 (from the
 *            denominator of the g.f.)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static const int init[14] = {0, 1, 2, 6, 18, 60, 184, 560, 1695, 5200,
                                 15956, 48916, 149664, 458048};
    seq_t w[14];
    int i;

    if (n < 14) {
        return (seq_t)init[n];
    }

    /* w[k] holds a(m - 14 + k) for the current m */
    for (i = 0; i < 14; i++) {
        w[i] = (seq_t)init[i];
    }

    for (idx_t m = 14; m <= n; m++) {
        /* w[13] = a(m-1), w[12] = a(m-2), ..., w[0] = a(m-14) */
        seq_t pos = w[13] * 2 + w[12] * 2 + w[10] * 10 + w[9] * 8
                  + w[4] * 4 + w[3] * 2 + w[1] * 2 + w[0];
        seq_t neg = w[8] * 2 + w[7] * 16 + w[6] * 10 + w[5] * 2;
        seq_t next = pos - neg;
        for (i = 0; i < 13; i++) {
            w[i] = w[i + 1];
        }
        w[13] = next;
    }

    return w[13];
}

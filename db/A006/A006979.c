/*
 * A006979  a(n) is the number of compositions of n in which the maximum part
 *          size is 5.
 * https://oeis.org/A006979
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = c5(n) - c4(n), where ck(n) = number of compositions of n
 *            with parts <= k: ck(n) = ck(n-1) + ... + ck(n-k), ck(0) = 1,
 *            ck(m) = 0 for m < 0. Equivalently G.f.: x^5 /
 *            ((1-x-x^2-x^3-x^4)*(1-x-x^2-x^3-x^4-x^5))
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 34, u64 n <= 67, u128 n <= 132,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t w4[4], w5[5];
    int k;
    idx_t i;
    seq_t s4, s5;

    if (n < 5) return 0;

    for (k = 0; k < 4; k++) w4[k] = 0;
    for (k = 0; k < 5; k++) w5[k] = 0;
    /* w4[0] holds c4(i-1), w4[1] holds c4(i-2), ...; start with c(0) = 1 */
    w4[0] = 1;
    w5[0] = 1;

    for (i = 1; i <= n; i++) {
        s4 = w4[0] + w4[1] + w4[2] + w4[3];
        s5 = w5[0] + w5[1] + w5[2] + w5[3] + w5[4];
        for (k = 3; k > 0; k--) w4[k] = w4[k - 1];
        w4[0] = s4;
        for (k = 4; k > 0; k--) w5[k] = w5[k - 1];
        w5[0] = s5;
    }

    return w5[0] - w4[0];
}

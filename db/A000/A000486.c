/*
 * A000486  One half of the number of permutations of [n] such that the
 *          differences have 4 runs with the same signs.
 * https://oeis.org/A000486
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 13*a(n-1) - 67*a(n-2) + 175*a(n-3) - 244*a(n-4) +
 *            172*a(n-5) - 48*a(n-6) for n >= 8, with a(2) = a(3) = a(4) = 0,
 *            a(5) = 16, a(6) = 150, a(7) = 926 (from G.f.
 *            2*x^5*(24*x^2-29*x+8) / ((x-1)^2*(2*x-1)^2*(3*x-1)*(4*x-1)))
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t w[6];
    idx_t i;
    if (n < 5) return 0;
    if (n == 5) return 16;
    if (n == 6) return 150;
    if (n == 7) return 926;
    /* w[0..5] = a(i-6) .. a(i-1), starting at i = 8 */
    w[0] = 0;   /* a(2) */
    w[1] = 0;   /* a(3) */
    w[2] = 0;   /* a(4) */
    w[3] = 16;  /* a(5) */
    w[4] = 150; /* a(6) */
    w[5] = 926; /* a(7) */
    for (i = 8; i <= n; i++) {
        seq_t pos = w[5] * 13 + w[3] * 175 + w[1] * 172;
        seq_t neg = w[4] * 67 + w[2] * 244 + w[0] * 48;
        seq_t next = pos - neg;
        w[0] = w[1];
        w[1] = w[2];
        w[2] = w[3];
        w[3] = w[4];
        w[4] = w[5];
        w[5] = next;
    }
    return w[5];
}

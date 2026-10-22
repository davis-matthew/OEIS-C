/*
 * A001658  Fibonomial coefficients.
 * https://oeis.org/A001658
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 13*a(n-1) + 104*a(n-2) - 260*a(n-3) - 260*a(n-4) +
 *            104*a(n-5) + 13*a(n-6) - a(n-7), a(0) = 1, a(1) = 13, a(2) =
 *            273, a(3) = 4641, a(4) = 85085, a(5) = 1514513, a(6) = 27261234
 *            (from g.f. 1/(1-13x-104x^2+260x^3+260x^4-104x^5-13x^6+x^7))
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t v[7];
    v[0] = (seq_t)1;
    v[1] = (seq_t)13;
    v[2] = (seq_t)273;
    v[3] = (seq_t)4641;
    v[4] = (seq_t)85085;
    v[5] = (seq_t)1514513;
    v[6] = (seq_t)27261234;
    if (n < 7) return v[n];
    /* v[6] = a(k-1), v[5] = a(k-2), ..., v[0] = a(k-7) */
    for (idx_t k = 7; k <= n; k++) {
        seq_t pos = v[6] * 13 + v[5] * 104 + v[2] * 104 + v[1] * 13;
        seq_t neg = v[4] * 260 + v[3] * 260 + v[0];
        seq_t next = pos - neg;
        v[0] = v[1];
        v[1] = v[2];
        v[2] = v[3];
        v[3] = v[4];
        v[4] = v[5];
        v[5] = v[6];
        v[6] = next;
    }
    return v[6];
}

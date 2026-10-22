/*
 * A004954  a(n) = round(n*phi^19), where phi is the golden ratio, A001622.
 * https://oeis.org/A004954
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = round(n*phi^19) = floor((9349*n + 1 +
 *            floor(4181*n*sqrt(5)))/2), since phi^19 = (L(19) +
 *            F(19)*sqrt(5))/2 with L(19) = 9349, F(19) = 4181
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 7, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t isqrt_seq(seq_t m) {
    seq_t t, x, y;
    int b = 0;
    if (m == 0) return m;
    t = m;
    while (t > 0) { t >>= 1; b++; }
    x = (seq_t)1;
    x <<= (b + 1) / 2;          /* x >= sqrt(m) */
    for (;;) {
        y = (x + m / x) / 2;
        if (y >= x) break;
        x = y;
    }
    return x;
}

seq_t a(idx_t n) {
    seq_t N = (seq_t)n;
    seq_t m, s;
    if (n == 0) return (seq_t)0;
    m = N * N * 87403805;       /* (4181*n)^2 * 5 */
    s = isqrt_seq(m);           /* floor(4181*n*sqrt(5)) */
    return (N * 9349 + 1 + s) / 2;
}

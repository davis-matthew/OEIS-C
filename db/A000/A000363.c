/*
 * A000363  Number of permutations of [n] with exactly 2 increasing runs of
 *          length at least 2.
 * https://oeis.org/A000363
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 14*a(n-1) - 75*a(n-2) + 196*a(n-3) - 263*a(n-4) +
 *            174*a(n-5) - 45*a(n-6), a(0) = a(1) = a(2) = a(3) = 0, a(4) = 5,
 *            a(5) = 61 (from G.f. -x^4*(9*x-5)/((x-1)^3*(3*x-1)^2*(5*x-1));
 *            equals (5^n-(2*n-1)*3^n+2*n^2-2*n-2)/16)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 15, u64 n <= 29, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t w[6];
    w[0] = 0; w[1] = 0; w[2] = 0; w[3] = 0; w[4] = 5; w[5] = 61;
    if (n < 6) return w[n];
    for (idx_t k = 6; k <= n; k++) {
        seq_t pos = w[5] * 14 + w[3] * 196 + w[1] * 174;
        seq_t neg = w[4] * 75 + w[2] * 263 + w[0] * 45;
        seq_t next = pos - neg;
        w[0] = w[1]; w[1] = w[2]; w[2] = w[3];
        w[3] = w[4]; w[4] = w[5]; w[5] = next;
    }
    return w[5];
}

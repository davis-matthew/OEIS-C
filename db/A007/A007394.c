/*
 * A007394  Number of strict 7th-order maximal independent sets in cycle
 *          graph.
 * https://oeis.org/A007394
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   G.f.: x^18*(9-7*x^2) / ((1-x^2)^2*(1-x^2-x^9)), i.e. with f(k) =
 *            f(k-2) + f(k-9), f(0) = 1, f(k<0) = 0; h(k) = h(k-2) + f(k);
 *            g(k) = g(k-2) + h(k) (zero for k < 0); a(n) = 9*g(n-18) -
 *            7*g(n-20)
 * Range:     u8 n <= 36, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n < 18) return 0;
    idx_t m = n - 18;
    seq_t fb[9], hb[2], gb[2];
    int i;
    for (i = 0; i < 9; i++) fb[i] = 0;
    hb[0] = 0; hb[1] = 0; gb[0] = 0; gb[1] = 0;
    seq_t gprev = 0, gcur = 0;
    idx_t k;
    for (k = 0; k <= m; k++) {
        seq_t fk;
        if (k == 0) {
            fk = 1;
        } else {
            seq_t fkm9 = fb[k % 9];
            seq_t fkm2 = (k >= 2) ? fb[(k - 2) % 9] : (seq_t)0;
            fk = fkm2 + fkm9;
        }
        fb[k % 9] = fk;
        hb[k % 2] += fk;
        gprev = gb[k % 2];
        gb[k % 2] += hb[k % 2];
        gcur = gb[k % 2];
    }
    return gcur * 9 - gprev * 7;
}

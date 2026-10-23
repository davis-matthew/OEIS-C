/*
 * A019561  Coordination sequence for C_5 lattice.
 * https://oeis.org/A019561
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k=0}^{5} p_k · binomial(n‑k+4,4) with p =
 *            {1,45,210,210,45,1}. This follows from the generating function
 *            (1+45x+210x²+210x³+45x⁴+x⁵)/(1‑x)⁵
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 119, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const seq_t P[6] = { (seq_t)1, (seq_t)45, (seq_t)210,
                            (seq_t)210, (seq_t)45, (seq_t)1 };

seq_t a(idx_t n) {
    seq_t sum = (seq_t)0;
    for (idx_t k = 0; k <= 5 && k <= n; ++k) {
        seq_t m = (seq_t)(n - k + 4);   /* m ≥ 4 for n ≥ k */
        if (m < 4) continue;
        seq_t c = m * (m - 1) * (m - 2) * (m - 3); /* product fits in seq_t */
        c /= 24;                                   /* gives binomial(m,4) */
        sum += P[k] * c;
    }
    return sum;
}

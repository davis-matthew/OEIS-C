/*
 * A055420  Number of points in N^n of norm <= 5.
 * https://oeis.org/A055420
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 22, u64 n <= 66, u128 n <= 364,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = Σ_{k=0}^{25} [x^k] (1 + x + x^4 + x^9 + x^16 + x^25)^n
   (number of n‑dimensional non‑negative integer vectors with Σ xi² ≤ 25) */
seq_t a(idx_t n)
{
    const idx_t MAX_DEG = 25;
    seq_t dp[26];
    seq_t newdp[26];
    /* initialize dp */
    for (idx_t i = 0; i <= MAX_DEG; ++i) dp[i] = 0;
    dp[0] = 1;
    static const idx_t base[6] = {0, 1, 4, 9, 16, 25};

    for (idx_t dim = 0; dim < n; ++dim) {
        for (idx_t i = 0; i <= MAX_DEG; ++i) newdp[i] = 0;
        for (idx_t d = 0; d <= MAX_DEG; ++d) {
            seq_t val = dp[d];
            if (val == 0) continue;
            for (idx_t k = 0; k < 6; ++k) {
                idx_t nd = d + base[k];
                if (nd <= MAX_DEG) newdp[nd] += val;
            }
        }
        for (idx_t i = 0; i <= MAX_DEG; ++i) dp[i] = newdp[i];
    }

    seq_t result = 0;
    for (idx_t i = 0; i <= MAX_DEG; ++i) result += dp[i];
    return result;
}

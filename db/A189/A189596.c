/*
 * A189596  Number of permutations of 1..n with displacements restricted to
 *          {-6,-5,-4,-2,-1,0,3}.
 * https://oeis.org/A189596
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of permutations of 1..n in which every element
 *            σ(i) satisfies σ(i) - i ∈ {-6,-5,-4,-2,-1,0,3}
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 27, u64 n <= 53, u128 n <= 104,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Allowed displacements */
static const int disp[7] = { -6, -5, -4, -2, -1, 0, 3 };

seq_t a(idx_t n)
{
    /* 10‑bit masks for positions i‑6 … i+3 */
    const unsigned int mask_bits = 1u << 10;          /* 1024 masks */
    seq_t dp[mask_bits], ndp[mask_bits];
    unsigned int i, j, d, m;
    int pos;
    seq_t total;
    
    /* initial mask: positions ≤ 0 are set (occupied / unavailable) */
    unsigned int init_mask = (1u << 6) - 1;          /* bits 0..5 set */
    for (j = 0; j < mask_bits; ++j) dp[j] = 0;
    dp[init_mask] = 1;
    
    for (i = 1; i <= n; ++i) {
        /* zero next array */
        for (j = 0; j < mask_bits; ++j) ndp[j] = 0;
        for (j = 0; j < mask_bits; ++j) {
            seq_t val = dp[j];
            if (!val) continue;
            for (d = 0; d < 7; ++d) {
                pos = (int)i + disp[d];
                if (pos < 1 || pos > (int)n) continue;
                m = disp[d] + 6;                 /* offset 0..9 */
                if (j & (1u << m)) continue;     /* already occupied */
                unsigned int newmask = j | (1u << m);
                unsigned int shifted = newmask >> 1;   /* shift window */
                ndp[shifted] += val;
            }
        }
        /* copy ndp back to dp */
        for (j = 0; j < mask_bits; ++j) dp[j] = ndp[j];
    }
    
    /* sum all states – each represents a completed permutation */
    total = 0;
    for (j = 0; j < mask_bits; ++j) total += dp[j];
    return total;
}

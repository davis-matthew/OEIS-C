/*
 * A010085  Weight distribution of [15,11,3] Hamming code of length 15 and
 *          minimal distance 3.
 * https://oeis.org/A010085
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = 0 for n > 15 a(n) = (C(15,n) + 15 * Σ_{i=0}^{min(7,n)}
 *            (-1)^{n-i} C(7,i) C(8,n-i)) / 16 for 0 ≤ n ≤ 15. This follows
 *            from the MacWilliams identity for the [15,11,3] Hamming code
 * Range:     u8 n <= 5, u16 n <= 15, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline int64_t binom_int(idx_t nn, idx_t kk) {
    if (kk > nn) return 0;
    if (kk > nn - kk) kk = nn - kk;
    int64_t res = 1;
    for (idx_t i = 1; i <= kk; ++i) {
        res = res * (int64_t)(nn - kk + i) / (int64_t)i;
    }
    return res;
}

seq_t a(idx_t n) {
    if (n > 15) return 0;

    int64_t bin15 = binom_int(15, n);
    int64_t sum   = 0;

    for (idx_t i = 0; i <= 7 && i <= n; ++i) {
        idx_t j = n - i;
        if (j > 8) continue;
        int64_t term = binom_int(7, i) * binom_int(8, j);
        if (j & 1) term = -term;
        sum += term;
    }

    int64_t val = (bin15 + 15 * sum) / 16;
    return (seq_t)val;
}

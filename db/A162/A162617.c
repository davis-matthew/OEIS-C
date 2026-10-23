/*
 * A162617  G.f. is the polynomial (1-x^3) * (1-x^6) * (1-x^9) * (1-x^12) *
 *          (1-x^15) * (1-x^18) * (1-x^21) * (1-x^24) * (1-x^27) * (1-x^30) /
 *          (1-x)^10.
 * https://oeis.org/A162617
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = Σ_{S ⊆ {3,6,…,30}} (-1)^{|S|} * C(n - ΣS + 9, 9) , where
 *            C(m,9) = 0 if m < 9. This is the inclusion–exclusion expansion
 *            of (1−x^3)(1−x^6)…(1−x^30)/(1−x)^10
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 33, u64 n <= 155, u128 n <= 155,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A162617 */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Pre‑compute subset sums and parities once. */
    static int sub_init = 0;
    static idx_t sub_sum[1 << 10];
    static int sub_parity[1 << 10];
    static const idx_t exps[10] = {3,6,9,12,15,18,21,24,27,30};

    if (!sub_init) {
        for (idx_t mask = 0; mask < (1u << 10); ++mask) {
            idx_t s = 0;
            int p = 0;
            for (int i = 0; i < 10; ++i) {
                if (mask & (1u << i)) {
                    s += exps[i];
                    p ^= 1;
                }
            }
            sub_sum[mask] = s;
            sub_parity[mask] = p;
        }
        sub_init = 1;
    }

    /* Accumulate contributions. */
    seq_t pos = 0;
    seq_t neg = 0;
    for (idx_t mask = 0; mask < (1u << 10); ++mask) {
        idx_t s = sub_sum[mask];
        if (s > n) continue;
        idx_t rem = n - s;            /* rem ≥ 0 */

        /* Compute binomial(rem+9, 9) multiplicatively. */
        seq_t val = 1;
        for (int k = 1; k <= 9; ++k) {
            val = (val * ((seq_t)rem + k)) / k;
        }

        if (sub_parity[mask] == 0)
            pos += val;
        else
            neg += val;
    }

    /* pos is always >= neg for the given generating function. */
    return pos - neg;
}

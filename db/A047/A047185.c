/*
 * A047185  Number of nonempty subsets of {1,2,...,n} in which exactly 1/4 of
 *          the elements are <= (n-2)/2.
 * https://oeis.org/A047185
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1}^{min(L,⌊H/3⌋)} binom(L,k) * binom(H,3k) where L =
 *            ⌊(n-2)/2⌋, H = n - L
 * Range:     u8 n <= 12, u16 n <= 21, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* L = floor((n-2)/2); clamp to 0 for n < 2 */
    idx_t L = (n >= 2u) ? (n - 2u) / 2u : 0u;
    /* H = n - L */
    idx_t H = n - L;

    /* kmax = min(L, floor(H/3)) */
    idx_t kmax = (L < H / 3u) ? L : H / 3u;
    if (kmax == 0u) return 0;

    /* Pre‑compute binomial coefficients C(H, j) for j = 0 … 3*kmax */
    size_t len = 3u * (size_t)kmax + 1u;
    seq_t *Hcomb = (seq_t *)malloc(len * sizeof(seq_t));
    if (!Hcomb) return 0;                 /* allocation failure – safe fallback */

    Hcomb[0] = 1;
    for (size_t j = 1u; j < len; ++j) {
        seq_t numerator = (seq_t)H - (seq_t)j + (seq_t)1;
        Hcomb[j] = Hcomb[j - 1] * numerator / (seq_t)j;
    }

    /* Sum contributions for each admissible k */
    seq_t total = 0;
    seq_t cL = 1;         /* C(L,0) */
    for (idx_t k = 1u; k <= kmax; ++k) {
        seq_t numeratorL = (seq_t)L - (seq_t)k + (seq_t)1;
        cL = cL * numeratorL / (seq_t)k;   /* C(L,k) */
        total += cL * Hcomb[3u * k];        /* C(H,3k) */
    }

    free(Hcomb);
    return total;
}

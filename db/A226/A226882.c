/*
 * A226882  Number of n-length words w over ternary alphabet {a,b,c} such that
 *          #(w,a) >= #(w,b) >= #(w,c) >= 1, where #(w,x) counts the letters x
 *          in word w.
 * https://oeis.org/A226882
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 41, u128 n <= 82,
 *            big any n
 * Helpers:   binomial_fast
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A226882: Number of n‑length words over {a,b,c}
   with #(a) ≥ #(b) ≥ #(c) ≥ 1.
   a(n) = Σ_{c=1}^{⌊n/3⌋} Σ_{b=c}^{⌊(n-c)/2⌋}
            binom(n, c) · binom(n-c, b)  (c,n,b integers) */
#include <stdbool.h>
#include "../utils/binomial_fast.h"

seq_t a(idx_t n)
{
    if (n < 3) return 0;              /* terms below the offset are irrelevant */

    seq_t total = 0;

    /* compute binom(n,c) incrementally over c to avoid repeated factorials */
    seq_t binom_n_c = 1;              /* binom(n,0) = 1, will be updated to binom(n,1) */
    /* start with c = 1 */
    binom_n_c = (seq_t)n;            /* binom(n,1) = n */

    for (idx_t c = 1; c <= n / 3; ++c) {
        idx_t m = n - c;                       /* remaining letters after c */
        idx_t max_b = m / 2;                   /* b ≤ a and b≥c → a = n‑b‑c ≥ b */

        /* compute binom(m,c) for this c */
        seq_t binom_m_c = binomial_fast(m, c);
        seq_t binom_m_b = binom_m_c;            /* will be updated in inner loop */

        for (idx_t b = c; b <= max_b; ++b) {
            /* add count for (a,b,c) with a = n - b - c (ensured ≥ b) */
            seq_t ways = binom_n_c * binom_m_b;
            total += ways;
            /* update binom(m,b+1) for next iteration, if any */
            if (b < max_b) {
                binom_m_b = binom_m_b * (m - b) / (b + 1);
            }
        }

        /* update binom_n_c to binom(n,c+1) for next outer loop iteration */
        if (c < n / 3) {
            binom_n_c = binom_n_c * (n - c) / (c + 1);
        }
    }

    return total;
}

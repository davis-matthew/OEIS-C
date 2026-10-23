/*
 * A151287  Number of walks within N^2 (the first quadrant of Z^2) starting at
 *          (0,0) and consisting of n steps taken from {(-1, -1), (-1, 0), (0,
 *          1), (1, -1), (1, 0)}.
 * https://oeis.org/A151287
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   binom, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A151287
        Number of walks within N^2 starting at (0,0) consisting of n steps
        taken from {(-1,-1),(-1,0),(0,1),(1,-1),(1,0)}.
    Derived formula:
        a(n) = ∑_{u=0}^{n}  S(u,n) * T(n-u)
    where
        k   = n – u
        T(k)      = binom(k, ⌊k/2⌋)          (number of Dyck prefixes of length k)
        S(u,n)    = 1  (k = 0)
                   = binom(n,u)/(u+1) * [ (u+1)*2^k – k*2^{k-1} ]   (k ≤ u)
                   = binom(n,u)/(u+1) * Σ_{d=0}^{u} (u-d+1)·binom(k,d)   (k > u)
*/

#include <stdlib.h>
#include "../utils/binom.h"   /* seq_t binom(idx_t a, idx_t b); */
#include "../utils/pow2.h"    /* seq_t pow2(idx_t exp); */

seq_t a(idx_t n)
{
    /* pre‑compute binomial coefficients C(n,u) for all u */
    seq_t *Cnu = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!Cnu) return 0;
    Cnu[0]  = 1;
    for (idx_t u = 0; u < n; ++u) {
        Cnu[u + 1] = Cnu[u] * (seq_t)(n - u) / (u + 1);
    }

    /* pre‑compute powers of two up to n-1 */
    seq_t *pow2_arr = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!pow2_arr) { free(Cnu); return 0; }
    pow2_arr[0] = 1;
    for (idx_t e = 1; e <= n; ++e) {
        pow2_arr[e] = pow2_arr[e - 1] * 2;
    }

    /* pre‑compute T(k) = binom(k, floor(k/2)) for all k */
    seq_t *T = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!T) { free(Cnu); free(pow2_arr); return 0; }
    for (idx_t k = 0; k <= n; ++k) {
        idx_t r = k / 2;
        T[k] = binom(k, r);
    }

    seq_t total = 0;

    for (idx_t u = 0; u <= n; ++u) {
        idx_t k = n - u;               /* number of non‑up steps */
        seq_t S;

        if (k == 0) {                  /* only up‐steps */
            S = 1;
        } else if (k <= u) {           /* k ≤ u  : closed form */
            seq_t num = (seq_t)(3 * u + 2 - n);
            seq_t den = (seq_t)(u + 1);
            S = Cnu[u] * pow2_arr[k - 1] * num / den;
        } else {                       /* k > u : partial sum over d ≤ u */
            seq_t sum = 0;
            seq_t bin_k_d = 1;         /* d = 0 */
            for (idx_t d = 0; d <= u; ++d) {
                seq_t term = bin_k_d * (seq_t)(u - d + 1);
                sum += term;
                if (d < u) {
                    seq_t num = (seq_t)(k - d);
                    seq_t den = (seq_t)(d + 1);
                    bin_k_d = (bin_k_d * num) / den;
                }
            }
            S = Cnu[u] * sum / (u + 1);
        }

        total += S * T[k];
    }

    free(Cnu);
    free(pow2_arr);
    free(T);
    return total;
}

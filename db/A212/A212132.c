/*
 * A212132  Number of (w,x,y,z) with all terms in {1,...,n} and w*x+y*z>=n^2.
 * https://oeis.org/A212132
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # { (w,x,y,z) ∈ [1..n]^4 | w*x + y*z ≥ n² } Observation:
 *            a(n) = n^4 – H(n²‑1) where H(T) = # { (w,x,y,z) | w*x + y*z ≤ T
 *            }. H(T) can be evaluated by counting pairs of products. Let P[p]
 *            = # {(w,x) | w*x = p}. Then H(T) = Σ_{p=1}^{T} P[p] · P_q where
 *            q ≤ T – p. This can be rewritten as H(T)...
 * Range:     u8 n <= 5, u16 n <= 29, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    idx_t nn = n;
    idx_t totalPairs = nn * nn;              /* n² */
    idx_t B = totalPairs - 1;                /* T = n² – 1 */

    size_t size = (size_t)totalPairs + 1;    /* array indices 0 … n² */
    uint16_t *cnt = (uint16_t*)calloc(size, sizeof(uint16_t));
    if (!cnt) return (seq_t)0;               /* allocation failure */

    /* Build P[p] = number of ordered pairs (w,x) with w·x = p */
    for (idx_t w = 1; w <= nn; ++w) {
        for (idx_t x = 1; x <= nn; ++x) {
            idx_t p = w * x;
            ++cnt[p];
        }
    }

    seq_t totalQuad = (seq_t)nn * (seq_t)nn * (seq_t)nn * (seq_t)nn; /* n⁴ */
    seq_t C = 0;               /* H(B) – number of quadruples with sum ≤ B */
    seq_t pref = 0;             /* running prefix sum of cnt */

    if (B > 0) {
        for (idx_t t = 0; t < B; ++t) {
            idx_t p = B - t;   /* corresponding first-pair product */
            C += (seq_t)cnt[p] * pref;
            pref += cnt[t + 1];
        }
    }

    seq_t result = totalQuad - C;
    free(cnt);
    return result;
}

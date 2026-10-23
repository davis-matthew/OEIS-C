/*
 * A171354  Number of 0..n-1 integer arrays v[1..3] of length 3 with all
 *          autocorrelation values sum(i){v[i]*v[i-k]} distinct for k in 0..2.
 * https://oeis.org/A171354
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n³ – ( 3n – 2 + Σ_{u,v≥1, gcd(u,v)=1, uv≤n‑1} * min(
 *            ⌊(n‑1)/(u(u+v))⌋, ⌊(n‑1)/(v(u+v))⌋, ⌊(n‑1)/(uv)⌋ ) ) * * The
 *            triples (a,b,c) with 0≤a,b,c≤n‑1 that violate the
 *            distinct‑autocorrelation * condition satisfy ab+bc=ac. These are
 *            counted by the sum above (plus the * trivial solutions a=c=0,...
 * Range:     u8 n <= 6, u16 n <= 32, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   gcd_idx_t_simple
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/gcd_idx_t_simple.h"

seq_t a(idx_t n)
{
    /* For n=1 the only triple is (0,0,0) which violates the condition. */
    if (n <= 1)
        return 0;

    const idx_t n1 = n - (idx_t)1;          /* n‑1, used frequently          */
    const seq_t nS = (seq_t)n;             /* for big integer arithmetic   */

    /* Count of trivial solutions: a=c=0 with any b, plus a=0,c>0 and a>0,c=0. */
    const seq_t zero_cases = (seq_t)3 * nS - (seq_t)2;

    /* Count of solutions with a>0 and c>0. */
    seq_t pos_pairs = 0;

    for (idx_t u = (idx_t)1; u <= n1; ++u) {
        /* v must satisfy u*v ≤ n1  ⇒  v ≤ n1 / u */
        const idx_t maxV = n1 / u;
        for (idx_t v = (idx_t)1; v <= maxV; ++v) {
            /* Skip non‑coprime pairs */
            if (gcd_idx_t_simple(u, v) != (idx_t)1)
                continue;

            /* Denominators for the bounds on k */
            const idx_t uv     = u * v;          /* u*v  */
            const idx_t upv    = u + v;          /* u+v  */
            const idx_t denom1 = u * upv;        /* u(u+v) */
            const idx_t denom2 = v * upv;        /* v(u+v) */

            /* Max k limited by each of the three inequalities */
            const idx_t k1 = n1 / denom1;
            const idx_t k2 = n1 / denom2;
            const idx_t k3 = n1 / uv;

            idx_t kmax = k1;
            if (k2 < kmax) kmax = k2;
            if (k3 < kmax) kmax = k3;

            if (kmax == (idx_t)0)
                continue; /* No solution for this pair */

            /* Each ordered pair (u,v) yields kmax distinct triples. */
            pos_pairs += (seq_t)kmax;
        }
    }

    /* Total violating triples */
    const seq_t total_bad = zero_cases + pos_pairs;

    /* Desired count a(n) = n³ – total_bad */
    const seq_t total = nS * nS * nS;      /* n³, computed with seq_t arithmetic */
    return total - total_bad;
}

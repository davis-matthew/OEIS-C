/*
 * A031360  Number of symmetrically inequivalent coincidence rotations of
 *          index 2n-1 in lattice D_4.
 * https://oeis.org/A031360
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) is multiplicative. Let m = 2*n – 1 (odd). Then a(n) =
 *            ∏_{p^e∥m} F(p,e) where F(p,e) = g_e + (p+1)·g_{e-1} + p·g_{e-2},
 *            with g_0 = 1, g_1 = p + p², and for k ≥ 2: g_k = (p +
 *            p²)·g_{k-1} – p³·g_{k-2}. (These formulas follow from the
 *            Dirichlet series in the OEIS entry.)
 * Range:     u8 n <= 7, u16 n <= 82, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 60 terms through n = 9791 (2026-11-01)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

static seq_t local_b(uint64_t p, idx_t e)
{
    if (e == 0) return (seq_t)1;

    seq_t pseq = (seq_t)p;
    seq_t p2   = pseq * pseq;      /* p² */
    seq_t A    = pseq + p2;         /* p + p² */
    seq_t p3   = p2   * pseq;       /* p³ */
    seq_t B    = p3;                /* p³ */

    /* g₀ = 1, g₁ = A = p + p² */
    seq_t g0 = 1;
    seq_t g1 = A;
    if (e == 1) {
        return g1 + (pseq + 1) * g0;
    }

    /* successive g_k, keeping g_{k-2} and g_{k-1}. */
    seq_t g_prev2 = g0;   /* g_{k-2} */
    seq_t g_prev1 = g1;   /* g_{k-1} */
    seq_t g_curr;

    for (idx_t k = 2; k <= e; ++k) {
        g_curr = A * g_prev1 - B * g_prev2;   /* g_k */
        if (k == e) {
            return g_curr + (pseq + 1) * g_prev1 + pseq * g_prev2;
        }
        g_prev2 = g_prev1;
        g_prev1 = g_curr;
    }
    return 1;   /* unreachable */
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* defined for n ≥ 1 */

    idx_t m = 2 * n - 1;           /* odd index in the aerated sequence */

    /* factorise m */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)m, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        result *= local_b(primes[i], (idx_t)exps[i]);
    }
    return result;
}

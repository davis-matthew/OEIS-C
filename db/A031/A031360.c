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
 * Helpers:   factor_u64, local_b
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/local_b.h"

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

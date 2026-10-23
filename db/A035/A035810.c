/*
 * A035810  Coordination sequence for lattice D*_50 (with edges defined by l_1
 *          norm = 1).
 * https://oeis.org/A035810
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 , n = 0 a(n) = Σ_{k=1}^{50} 2^k * C(50,k) * C(n-1,k-1)
 *            + 2^50 * C(n+24,49) , n ≥ 1
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"   /* binomial(idx_t, idx_t) */
#include "../utils/pow2.h"       /* pow2(idx_t) */

seq_t a(idx_t n)
{
    /* The offset is 0, so a(0) is the first term. */
    if (n == 0) return (seq_t)1;

    const idx_t N = 50;
    seq_t sum = (seq_t)0;

    /* First part of the formula. */
    for (idx_t k = 1; k <= N; ++k) {
        /* C(N,k) and 2^k are constants for fixed N. */
        seq_t bk     = binomial(N, k);
        seq_t twok   = pow2(k);
        seq_t dm1k1  = binomial(n - 1, k - 1);
        sum += twok * bk * dm1k1;
    }

    /* Second part of the formula: 2^50 * C(n+24,49). */
    seq_t term2 = pow2(N) * binomial(n + 24, N - 1);

    return sum + term2;
}

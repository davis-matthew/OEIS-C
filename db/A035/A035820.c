/*
 * A035820  Coordination sequence for lattice D*_70 (with edges defined by l_1
 *          norm = 1).
 * https://oeis.org/A035820
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1}^{70} 2^k * C(70,k) * C(n-1,k-1) + 2^70 *
 *            C(n+34,69) (a(0)=1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/binomial.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;

    const idx_t N = 70;
    seq_t sum = 0;

    for (idx_t k = 1; k <= N; ++k) {
        seq_t cnk  = binomial(N, k);          /* C(70,k) */
        seq_t cmk  = binomial(n - 1, k - 1);   /* C(n-1,k-1) */
        seq_t term = cnk * cmk;
        term *= pow2(k);                      /* 2^k */
        sum += term;
    }

    seq_t bin_second = binomial(n + 34, 69);  /* C(n+34,69) */
    seq_t second = 0;
    if (bin_second != (seq_t)0) {
        second = bin_second * pow2(70);        /* 2^70 * C(n+34,69) */
    }

    return sum + second;
}

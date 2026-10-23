/*
 * A035819  Coordination sequence for lattice D*_68 (with edges defined by l_1
 *          norm = 1).
 * https://oeis.org/A035819
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{k=1}^{68} 2^k·binomial(68,k)·binomial(n-1,k-1) +
 *            2^68·binomial(n+33,67)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    /* constant value 68 */
    const idx_t N_CONST = 68;
    static int initialized = 0;
    static seq_t binom_nk[69];   /* binom(68,k) for k=0..68 */
    static seq_t pow2_k[69];     /* 2^k for k=0..68 */

    if (!initialized) {
        /* precompute binomial(68,k) */
        binom_nk[0] = 1;
        for (idx_t i = 1; i <= N_CONST; ++i) {
            /* binom_nk[i] = binom_nk[i-1] * (N_CONST - i + 1) / i */
            seq_t num = binom_nk[i-1] * (N_CONST - i + 1);
            binom_nk[i] = num / i;
        }
        /* precompute powers of two */
        pow2_k[0] = 1;
        for (idx_t i = 1; i <= N_CONST; ++i)
            pow2_k[i] = pow2_k[i-1] * 2;
        initialized = 1;
    }

    seq_t sum = 0;
    idx_t kmax = N_CONST;
    if (n < kmax)
        kmax = n;

    for (idx_t k = 1; k <= kmax; ++k) {
        seq_t term = pow2_k[k] * binom_nk[k];
        term = term * binomial(n - 1, k - 1);
        sum += term;
    }

    /* second term: 2^68 * binomial(n+33, 67) */
    seq_t pow2_68 = pow2_k[N_CONST];
    seq_t term2 = pow2_68 * binomial(n + 33, 67);

    return sum + term2;
}

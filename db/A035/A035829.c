/*
 * A035829  Coordination sequence for lattice D*_88 (with edges defined by l_1
 *          norm = 1).
 * https://oeis.org/A035829
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(m) = Σ_{k=0}^{88} 2^k * C(88,k) * C(m-1,k-1) + 2^88 *
 *            C((88+2*m)/2 - 1, 87)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binomial.h"

#define N 88

seq_t a(idx_t n)
{
    /* Pre‑compute 2^k and 2^k*C(N,k) once. */
    static bool init = false;
    static seq_t pow2[N + 1];
    static seq_t factor[N + 1];   /* factor[k] = 2^k * C(N,k) */

    if (!init) {
        pow2[0] = 1;
        for (idx_t i = 1; i <= N; ++i)
            pow2[i] = pow2[i - 1] * (seq_t)2;

        /* Compute C(N,k) for k=0..N and combine with 2^k. */
        for (idx_t k = 0; k <= N; ++k) {
            seq_t binom_n_k = binomial((idx_t)N, k);
            factor[k] = pow2[k] * binom_n_k;
        }
        init = true;
    }

    /* Special case a(0) = 1. */
    if (n == 0)
        return 1;

    /* Part 1: Σ_{k=1}^{min(N,n)} 2^k*C(N,k)*C(n-1,k-1) */
    idx_t maxk = (n <= N ? n : N);
    seq_t sum1 = 0;
    for (idx_t k = 1; k <= maxk; ++k) {
        seq_t binom = binomial(n - 1, k - 1);   /* C(n-1,k-1) */
        sum1 += factor[k] * binom;
    }

    /* Part 2: 2^N * C(n+43, N-1)   ("n+43" comes from (88+2*n)/2 - 1) */
    seq_t sum2 = pow2[N] * binomial(n + 43, (idx_t)N - 1);

    return sum1 + sum2;
}

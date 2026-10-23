/*
 * A035803  Coordination sequence for lattice D*_36 (with edges defined by l_1
 *          norm = 1).
 * https://oeis.org/A035803
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1}^{36} 2^k * C(36,k) * C(n-1,k-1) + 2^36 *
 *            C(n+17,35), a(0) = 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function is valid for all n ≥ 0. */

#include "../utils/binomial.h"
#include <stdbool.h>

#define N 36ULL  /* fixed dimension */

seq_t a(idx_t n)
{
    /* Special case for n = 0 */
    if (n == 0U)
        return 1;

    /* Static arrays for pre‑computed values (initialized once) */
    static seq_t binom_N_k[N + 1];
    static seq_t pow2k[N + 1];
    static bool initialized = false;

    if (!initialized) {
        binom_N_k[0] = 1;          /* C(N,0) = 1 */
        pow2k[0] = 1;              /* 2^0 = 1 */
        for (idx_t i = 1; i <= N; ++i) {
            binom_N_k[i] = binomial(N, i);              /* C(N,i) */
            pow2k[i] = pow2k[i - 1] + pow2k[i - 1];     /* 2^i */
        }
        initialized = true;
    }

    seq_t sum = 0;
    /* Sum over k = 1 .. N (k = 0 term is zero for n ≥ 1) */
    for (idx_t k = 1; k <= N; ++k) {
        /* C(n-1,k-1) is zero when k-1 > n-1 */
        seq_t binom_mk = binomial(n - 1, k - 1);
        if (binom_mk == 0)
            continue;  /* skip terms that contribute nothing */

        seq_t term = pow2k[k] * binom_N_k[k] * binom_mk;
        sum += term;
    }

    /* Second part: 2^N * C(n+17, N-1) */
    seq_t second = pow2k[N] * binomial(n + 17, N - 1);

    return sum + second;
}

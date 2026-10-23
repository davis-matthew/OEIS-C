/*
 * A035804  Coordination sequence for lattice D*_38 (with edges defined by l_1
 *          norm = 1).
 * https://oeis.org/A035804
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 7, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula from OEIS A035804:
 *   a(n) = Σ_{k=1}^{38} 2^k * C(38,k) * C(n-1,k-1) + 2^38 * C(n+18,37)
 *   with the convention a(0)=1.  Indexing starts at n = 0.
 */

#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    const idx_t N = 38;                     /* fixed dimension */
    static seq_t pow2_arr[39];              /* 2^k for k=0..38 */
    static seq_t binom_arr[39];             /* C(38,k) for k=0..38 */
    static bool init = false;               /* one‑time initialization */

    if (!init) {
        for (idx_t i = 0; i <= N; ++i) {
            pow2_arr[i] = pow2(i);
            binom_arr[i] = binomial(N, i);
        }
        init = true;
    }

    if (n == 0)               /* a(0) = 1 */
        return (seq_t)1;

    seq_t sum = 0;

    /* First summation part, k = 1 … N, skips k=0 which would give 0 anyway */
    for (idx_t k = 1; k <= N; ++k) {
        seq_t term = pow2_arr[k];
        term *= binom_arr[k];
        term *= binomial(n - 1, k - 1);
        sum += term;
    }

    /* Second term: 2^38 * C(n+18, 37) */
    seq_t term2 = pow2_arr[N];
    term2 *= binomial(n + 18, N - 1);
    sum += term2;

    return sum;
}

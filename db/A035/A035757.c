/*
 * A035757  Coordination sequence for C_20 lattice.
 * https://oeis.org/A035757
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = coefficient of x^(2n) in ((1+x)/(1-x))^20
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 5, u64 n <= 19, u128 n <= 203,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*           = Σ_{k=0}^{min(20,2n)} C(20,k) * C(19+2n-k,19) */

#include <stdint.h>   /* for uint64_t */

static seq_t binom(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t r = 1;
    for (idx_t i = 1; i <= k; ++i) {
        r *= (seq_t)(n - k + i);
        r /= (seq_t)i;
    }
    return r;
}

seq_t a(idx_t n)
{
    /* Pre‑compute C(20,k) once */
    static seq_t binom20[21];
    static int init = 0;
    if (!init) {
        for (idx_t k = 0; k <= 20; ++k) {
            binom20[k] = binom(20, k);
        }
        init = 1;
    }

    if (n == 0) return (seq_t)1;   /* a(0) = 1 */

    idx_t maxk = (2u * n < 20) ? 2u * n : 20;   /* min(20,2n) */
    seq_t sum = 0;

    for (idx_t k = 0; k <= maxk; ++k) {
        idx_t m = 19 + 2u * n - k;          /* m = 19 + 2n - k >= 19 */
        seq_t term = binom20[k] * binom(m, 19);
        sum += term;
    }
    return sum;
}

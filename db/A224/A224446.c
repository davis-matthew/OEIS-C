/*
 * A224446  Denominators of certain rationals approximating sqrt(3).
 * https://oeis.org/A224446
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of the rational * r(n) = 2 * (1 - 2 *
 *            ∑_{k=1..n} C(k-1) / 2^(4k)), * where C(m) is the m‑th Catalan
 *            number. * a(n) is a power of two
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Special case n = 0 */
    if (n == 0)
        return (seq_t)1;

    /* Compute N = Σ_{k=1..n} C(k-1) * 2^(4(n-k)) */
    seq_t N = 0;
    seq_t C = 1;          /* C(0) = 1 */
    for (idx_t k = 1; k <= n; ++k) {
        /* term = C(k-1) * 2^(4*(n-k)) */
        seq_t term = C << (4 * (n - k));
        N += term;

        /* Update Catalan number for next iteration    */
        if (k < n) {
            /* Ck = C_{k-1} * 2*(2k-1) / (k+1) */
            seq_t num = C * (seq_t)(2 * (2 * k - 1));
            C = num / (seq_t)(k + 1);
        }
    }

    /* M = 2^(4n) - 2*N  */
    seq_t a_pow = pow2(4 * n); /* 2^(4n) */
    seq_t M = a_pow - (N << 1);   /* subtract 2*N */

    /* Compute v2(M) – number of trailing zero bits */
    idx_t v2M = 0;
    while ((M & 1) == 0) {
        M >>= 1;
        ++v2M;
    }

    /* Exponent of the denominator: 4n - 1 - v2(M) */
    idx_t exp = 4 * n - 1 - v2M;

    return pow2(exp);  /* return 2^exp */
}

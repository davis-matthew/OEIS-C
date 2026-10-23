/*
 * A163929  Denominators of the higher order exponential integral constants
 *          alpha(2,n).
 * https://oeis.org/A163929
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of α(2,n) where α(2,n) = ½·(∑_{p=1}^{n-1}1/p⁴
 *            + (∑_{p=1}^{n-1}1/p²)²)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 13, u128 n <= 16,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Compute the denominator of α(2,n) for n ≥ 1.  */
#include "../utils/gcd.h"   /* seq_t gcd(seq_t a, seq_t b) */

seq_t a(idx_t n)
{
    if (n <= 1) return 1;           /* a(1) = 1 */

    /* lcm of 1..n‑1 */
    seq_t base_lcm = 1;
    for (idx_t i = 2; i < n; ++i) {
        seq_t ii = (seq_t)i;
        seq_t g = gcd(base_lcm, ii);
        base_lcm = (base_lcm / g) * ii;
    }

    /* L2 = lcm², L4 = lcm⁴  (these fit in seq_t for the n where the
       denominator is required to fit in the build type) */
    seq_t L2 = base_lcm * base_lcm;
    seq_t L4 = L2 * L2;

    /* S2_num / L2   with S2_num = Σ L2 / d²  (d = 1..n-1)   */
    seq_t S2_num = 0;
    /* S4_num / L4   with S4_num = Σ L4 / d⁴  (d = 1..n-1)   */
    seq_t S4_num = 0;

    for (idx_t d = 1; d < n; ++d) {
        seq_t d_sq = ((seq_t)d) * ((seq_t)d);
        S2_num += L2 / d_sq;

        seq_t d_four = d_sq * d_sq;
        S4_num += L4 / d_four;
    }

    /* α(2,n) = (S4 + S2²) / 2  with common denominator L4   */
    seq_t sum_num = S4_num + S2_num * S2_num;   /* numerator */
    seq_t denom   = 2 * L4;                    /* denominator before reduction */

    seq_t g = gcd(sum_num, denom);
    sum_num /= g;
    denom   /= g;

    return denom;   /* required denominator of α(2,n) */
}

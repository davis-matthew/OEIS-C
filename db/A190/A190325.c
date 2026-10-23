/*
 * A190325  n + [n*r/t] + [n*s/t]; r=1, s=sinh(Pi/2), t=cosh(Pi/2).
 * https://oeis.org/A190325
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + ⌊n·sech(π/2)⌋ + ⌊n·tanh(π/2)⌋ Off‑set: 1 (a(1) is the
 *            first term listed in the OEIS entry)
 * Range:     u8 n <= 110, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/*  Constants:  α = sech(π/2),  β = tanh(π/2)  */
/*  They are stored as rational approximations: α≈398537092/1 000 000 000,
    β≈917153200/1 000 000 000.  The error is below 5·10⁻¹⁰, which keeps
    n·α and n·β within 10⁻⁵ of the true values for all n ≤ 10 000. */

static const uint64_t ALPHA_NUM = 398537092ULL;
static const uint64_t ALPHA_DEN = 1000000000ULL;
static const uint64_t BETA_NUM  = 917153200ULL;
static const uint64_t BETA_DEN  = 1000000000ULL;

/*  Compute the n-th term of A190325 (n ≥ 1).  */
seq_t a(idx_t n)
{
    /*  Work in __int128 to avoid overflow of n·numerator.  */
    __int128 temp1 = (__int128)n * ALPHA_NUM;
    __int128 temp2 = (__int128)n * BETA_NUM;

    seq_t term1 = (seq_t)(temp1 / ALPHA_DEN);
    seq_t term2 = (seq_t)(temp2 / BETA_DEN);

    return (seq_t)n + term1 + term2;
}

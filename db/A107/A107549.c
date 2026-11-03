/*
 * A107549  Coordination sequence for B_16 lattice.
 * https://oeis.org/A107549
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k=0}^{min(n,16)} c_k * C(n-k+15,15) where the
 *            coefficients c_k are the coefficients of the numerator of the
 *            generating function G(x) = (x^16 + 4976*x^15 + 233976*x^14 +
 *            4257488*x^13 + 38523420*x^12 + 193440624*x^11 + 573006280*x^10 +
 *            1036952400*x^9 + 1166597190*x^8 + 818649040*x^7 + 354...
 * Range:     u8 none, u16 n <= 0, u32 n <= 5, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Validated: partial, pass, 19 terms through n = 18 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* compute binomial (n+15 choose 15) for n >= 0 */
static seq_t binom_n_plus_15_choose_15(idx_t n)
{
    /* We compute C(n+15, 15) multiplicatively.
       The division is exact at each step. */
    seq_t res = 1;
    /* n+15 is at most around 10^4 + 15 in typical usage; intermediate
       values may exceed 64 bits but fit into the seq_t type. */
    for (idx_t i = 1; i <= 15; ++i)
    {
        res = res * (seq_t)(n + 15 - (i - 1));
        res = res / (seq_t)i;
    }
    return res;
}

/* coefficients of the numerator, indexed from x^0 to x^16 */
static const seq_t COEF[17] = {
    1ULL,          /* x^0   */
    496ULL,        /* x^1   */
    40440ULL,      /* x^2   */
    1104208ULL,    /* x^3   */
    13869596ULL,   /* x^4   */
    92517360ULL,   /* x^5   */
    354721224ULL,  /* x^6   */
    818649040ULL,  /* x^7   */
    1166597190ULL,/* x^8   */
    1036952400ULL,/* x^9   */
    573006280ULL, /* x^10  */
    193440624ULL, /* x^11  */
    38523420ULL,  /* x^12  */
    4257488ULL,   /* x^13  */
    233976ULL,    /* x^14  */
    4976ULL,      /* x^15  */
    1ULL          /* x^16  */
};

seq_t a(idx_t n)
{
    seq_t sum = 0;
    /* We only sum over k where n-k >= 0 */
    for (idx_t k = 0; k <= 16 && k <= n; ++k)
    {
        seq_t bin = binom_n_plus_15_choose_15(n - k);
        sum += COEF[k] * bin;
    }
    return sum;
}

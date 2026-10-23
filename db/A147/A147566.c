/*
 * A147566  Triangle, T(n, k) = coefficients [x^k]( p(x,n) ), where p(x, n) =
 *          (x+1)^n for n < 2, otherwise (x+1)^n + x*((1+x)^(n-2) +
 *          2^(n-2)*(1-x)^(n-1)*LerchPhi(x, 2-n, 1/2)), read by rows.
 * https://oeis.org/A147566
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 48, u32 n <= 95, u64 n <= 216,
 *            u128 n <= 508, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) is the n‑th entry of the triangle T(r,k) read row by row.
 * T(r,k) is the coefficient of x^k in
 *
 *   p(x,r) = (x+1)^r                                 for r < 2
 *   p(x,r) = (x+1)^r + x*((1+x)^(r-2) + 2^(r-2)*(1-x)^(r-1)*LerchPhi(x,2-r,1/2))
 *              for r ≥ 2
 *
 * The coefficients can be computed by the closed form
 *
 *     T(r,k) = C(r,k)
 *           + (k>0   ? C(r-2,k-1)            : 0)
 *           + Σ_{j=0}^{min(r-1,k-1)} (-1)^j * C(r-1,j)
 *                                     * (2k-2j-1)^(r-2)
 *
 * where C(a,b) is the binomial coefficient.
 * The function a(idx_t n) returns the n‑th element of the sequence,
 * i.e. a(n) = T(r,k) where r*(r+1)/2 ≤ n < (r+1)(r+2)/2 and
 * k = n – r*(r+1)/2.
 */

#include <stdbool.h>  /* for bool */

/* ------------------------------------------------------------------ */
/* Helper: multiplicative binomial coefficient C(n,k)                */
static seq_t binom_local(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        /* multiply first to keep the division exact */
        res = res * (n - k + i) / i;
    }
    return res;
}

/* ------------------------------------------------------------------ */
/* Helper: fast exponentiation of a seq_t base to an unsigned exponent */
static seq_t pow_seq_local(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp) {
        if (exp & 1) result *= base;
        exp >>= 1;
        if (exp) base *= base;
    }
    return result;
}

/* ------------------------------------------------------------------ */
/* Main sequence function                                            */
seq_t a(idx_t n)
{
    /* Determine the row r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2   */
    idx_t r = 0;
    while (((r + 1U) * (r + 2U) / 2U) <= n) ++r;

    /* Column index within the row   */
    idx_t offset = r * (r + 1U) / 2U;
    idx_t k = n - offset;

    /* Use symmetry T(r,k) = T(r,r-k) to keep k small               */
    if (k > r - k) k = r - k;

    /* Base binomial term C(r,k)                                      */
    seq_t val = binom_local(r, k);

    /* Add the second term C(r-2,k-1) if applicable                  */
    if (r >= 2U && k >= 1U) {
        val += binom_local(r - 2U, k - 1U);
    }

    /* Add the alternating sum term if r >= 2 and k >= 1             */
    if (r >= 2U && k >= 1U) {
        idx_t maxj = r - 1U;
        if (k - 1U < maxj) maxj = k - 1U;
        for (idx_t j = 0U; j <= maxj; ++j) {
            seq_t coef = binom_local(r - 1U, j);
            idx_t base_int = 2U * k - 2U * j - 1U;   /* ≥ 1 for j ≤ k-1 */
            seq_t power = pow_seq_local((seq_t)base_int, r - 2U);
            seq_t term = coef * power;
            if (j & 1U) {
                val -= term;
            } else {
                val += term;
            }
        }
    }

    return val;
}

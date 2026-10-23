/*
 * A157981  Triangle: r=23;l=7;m(r,l,n)=(r - l)*IdentityMatrix[n] + l*Table[1,
 *          {i, 1, n}, {j, 1, n}].
 * https://oeis.org/A157981
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, uned, less
 * Formula:   For each r ≥ 0 let M_r = (23‑7)I_r + 7J_r (I_r = identity, J_r =
 *            all‑ones). Let P_r(x) = det(xI_r – M_r) = x^r + c_{r‑1}x^{r‑1} +
 *            … + c_0. Then the sequence is read row‑wise: a(r(r+1)/2 + k) =
 *            (‑1)^r * c_k (0 ≤ k ≤ r). The table rows are therefore the
 *            coefficients of (x‑(23+7(r‑1)))(x‑16)^{r‑1}
 * Range:     i8 n <= 2, i16 n <= 9, i32 n <= 35, i64 n <= 35, i128 n <= 35,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include the binomial helper from the utilities library */
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2 */
    idx_t low = 0, high = 1;
    while ( (__int128)high * (high + 1) / 2 <= n ) {
        high <<= 1;
        if (high == 0) break;   /* safety for overflow */
    }
    while (low + 1 < high) {
        idx_t mid = low + (high - low) / 2;
        if ( (__int128)mid * (mid + 1) / 2 <= n ) low = mid;
        else                                       high = mid;
    }
    idx_t r = low;
    idx_t offset = r * (r + 1) / 2;
    idx_t k = n - offset;

    /* If the term is the last in its row we just return (-1)^r */
    if (k == r) {
        return (r & 1) ? -(seq_t)1 : (seq_t)1;
    }

    /* Otherwise compute the coefficient c_k of λ^k in P_r(λ) */
    /* P_r(λ) = (λ‑16)^(r‑1) (λ‑(7r+16))   (since r‑l = 16, b = 7r+16) */
    idx_t s = r - 1 - k;                /* exponent of (‑16) in the formula */
    seq_t powNeg16 = 1;                 /* (‑16)^s */
    for (idx_t i = 0; i < s; ++i) {
        powNeg16 *= (seq_t)-16;
    }

    seq_t bin1 = binom(r - 1, s + 1);   /* C(r-1, s+1) */
    seq_t bin0 = binom(r - 1, s);       /* C(r-1, s)   */
    seq_t term1   = bin1 * powNeg16 * (seq_t)-16;            /* bin1 * (‑16)^(s+1) */
    seq_t b = ((seq_t)7 * r) + (seq_t)16;                  /* 7r + 16 */
    seq_t term2   = b * bin0 * powNeg16;                    /* b * bin0 * (‑16)^s */
    seq_t c_k     = term1 - term2;                          /* coefficient of λ^k */

    /* Apply the row sign factor (‑1)^r */
    if (r & 1) {
        c_k = -c_k;
    }
    return c_k;
}

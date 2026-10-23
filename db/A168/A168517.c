/*
 * A168517  Expansion of g.f. (1/2)*( a*(1+x)^n + b*(1-x)^(n+2)*LerchPhi(x,
 *          -n-1, 1) + c*2^(n+1)*(1-x)^(n+1)*LerchPhi(x, -n, 1/2) ), where a =
 *          -1, b = 1, and c = 1, read by rows.
 * https://oeis.org/A168517
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where r = floor((sqrt(8*n+1)-1)/2), k = n -
 *            r*(r+1)/2, and T(r,k) = 1/2*( -binom(r,k) + Σ_{j=0}^{k}
 *            (-1)^{k-j} [ binom(r+2, k-j)*(j+1)^{r+1} + 2*binom(r+1,
 *            k-j)*(2*j+1)^{r} ] )
 * Range:     u8 n <= 11, u16 n <= 29, u32 n <= 69, u64 n <= 176,
 *            u128 n <= 446, big any n
 * Helpers:   binom, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    /* Determine row r and position k within that row (row-major indexing). */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    idx_t offset = r * (r + 1) / 2;      /* first index of row r */
    idx_t k = n - offset;                /* position within the row (0‑based) */

    /* a = -1, b = 1, c = 1 in the defining sums. */
    seq_t bin_nk = binom(r, k);

    seq_t pos_sum = (seq_t)0;   /* accumulates terms with positive sign */
    seq_t neg_sum = (seq_t)0;   /* accumulates terms with negative sign */

    /* a*binom(r,k) contributes a negative term (-1*binom). */
    neg_sum += bin_nk;

    /* Sum over j = 0 .. k of the two parts defined in the OEIS formula. */
    for (idx_t j = 0; j <= k; ++j)
    {
        idx_t d = k - j;                    /* exponent for the sign factor */

        /* term1 = binom(r+2, d) * (j+1)^(r+1) */
        seq_t term1 = binom(r + 2, d);
        seq_t pow1  = pow_seq((seq_t)(j + 1), (idx_t)(r + 1));
        term1 = term1 * pow1;

        /* term2 = 2 * binom(r+1, d) * (2*j+1)^r */
        seq_t term2 = binom(r + 1, d);
        seq_t pow2  = pow_seq((seq_t)(2 * j + 1), (idx_t)r);
        term2 = term2 * pow2;
        term2 = term2 * ((seq_t)2);

        seq_t tsum = term1 + term2;          /* inner sum for this j */

        /* Add or subtract based on the sign factor (-1)^(k-j). */
        if ((d & 1u) == 0)
            pos_sum += tsum;
        else
            neg_sum += tsum;
    }

    /* net = a*binom + total sum of inner terms                       */
    seq_t net = pos_sum - neg_sum;

    /* Divide by 2 (the 1/2 factor in the definition).            */
    return net / (seq_t)2;
}

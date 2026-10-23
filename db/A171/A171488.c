/*
 * A171488  Riordan array (f(x), x*f(x)) where f(x) is the g.f. of
 *          A005773(n+1)= 1,2,5,13,35,96,267,...
 * https://oeis.org/A171488
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(row, col) where row = floor((sqrt(8*n+1)-1)/2) col = n
 *            - row*(row+1)/2 and T(r,k) = (k+1) * Σ_{j=0}^{r-k} binom(2*j+k,
 *            j) * (-1)^j * 3^{r-j-k} * binom(r+1, j+k+1) / (r+1) The division
 *            is exact
 * Range:     u8 n <= 9, u16 n <= 35, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* We use the binomial coefficient routine from ../utils/binom.h */
#include "../utils/binom.h"

static idx_t compute_row(idx_t n)
{
    /* Find the unique r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;
    return r;
}

/* Compute 3^e with an integer loop (e <= r <= 140, small). */
static seq_t pow3_u64(idx_t e)
{
    seq_t res = 1;
    for (idx_t i = 0; i < e; ++i)
        res *= 3;
    return res;
}

seq_t a(idx_t n)
{
    /* Determine row r and column k from the linear index n */
    idx_t r = compute_row(n);
    idx_t first = r * (r + 1) / 2;   /* first index in row r */
    idx_t k = n - first;

    /* Compute the sum part of the T(r,k) formula */
    seq_t pos_sum = 0;   /* sum of terms with even j (sign +) */
    seq_t neg_sum = 0;   /* sum of terms with odd j  (sign -) */

    for (idx_t j = 0; j <= r - k; ++j) {
        seq_t bin1 = binom(2 * j + k, j);
        seq_t bin2 = binom(r + 1, j + k + 1);
        seq_t pow3 = pow3_u64(r - j - k);
        seq_t term = bin1 * pow3 * bin2;
        if ((j & 1) == 0)
            pos_sum += term;
        else
            neg_sum += term;
    }

    seq_t sum = pos_sum - neg_sum;                /* difference is non‑negative */
    seq_t result = ((k + 1) * sum) / (r + 1);     /* exact division */

    return result;
}

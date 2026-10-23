/*
 * A104709  Triangle read by rows: T(n,k) = Sum_{j=0..n} 2^(n-j)*binomial(j,k)
 *          for n >= 0 and 0 <= k <= n; also, Riordan array
 *          (1/((1-x)*(1-2*x)), x/(1-x)).
 * https://oeis.org/A104709
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   binom, pow2_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = T(r,k) where r(r+1)/2 ≤ n < (r+1)(r+2)/2,
 * k = n - r(r+1)/2, and T(r,k) = Σ_{j=0}^{r-k} C(r-j,k)·2^j
 * (the triangle read by rows, first row T(0,0), second row T(1,0),T(1,1), …)
 */

#include <stdint.h>
#include "../utils/binom.h"   /* binom(idx_t N, idx_t K) */
#include "../utils/pow2_seq.h"

seq_t a(idx_t n)
{
    /* Find row r: r(r+1)/2 ≤ n < (r+1)(r+2)/2 */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    /* Column within that row */
    idx_t k = n - r * (r + 1) / 2;

    /* Compute T(r,k) = Σ_{j=0}^{r-k} C(r-j,k) * 2^j */
    seq_t sum = 0;
    for (idx_t j = 0; j <= r - k; ++j)
    {
        seq_t term = binom(r - j, k) * pow2_seq(j);
        sum += term;
    }

    return sum;
}

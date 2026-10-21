/*
 * A095677  Triangle T(n,k), 0<=k<=n, read by rows, defined by Sum_{k = 0..n}
 *          T(n,k)*x^k = Sum_{k = 0..n} binomial(n,k)*(x+k)^n.
 * https://oeis.org/A095677
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the flattened OEIS triangle T(n,k) defined by *
 *            ∑_{k=0..n} T(n,k)·x^k = ∑_{k=0..n} binomial(n,k)·(x+k)^n . *
 *            a(n) is obtained by concatenating the rows of this triangle: * 1
 *            ; 1,2 ; 6,8,4 ; 54,72,36,8 ; …
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Locate row r : cumulative(r-1) ≤ n < cumulative(r),
       where cumulative(r) = (r+1)(r+2)/2. */
    idx_t lo = 0, hi = 1;
    while ((__int128)(hi + 1) * (hi + 2) / 2 <= n) hi <<= 1;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if ((__int128)(mid + 1) * (mid + 2) / 2 > n)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t r = hi;                           /* row number */
    idx_t prev_cum = (r == 0) ? 0 : ((unsigned __int128)r * (r + 1)) / 2;
    idx_t k = n - prev_cum;                 /* column within the row */

    /* Compute T(r,k) = binomial(r,k) * Σ_{j=0..r} binomial(r,j)·j^(r-k). */
    seq_t bin_ik = binomial(r, k);
    seq_t sum_terms = (seq_t)0;
    idx_t exponent = r - k;
    for (idx_t j = 0; j <= r; ++j) {
        seq_t bin_j = binomial(r, j);
        seq_t pow_j = (j == 0) ? (exponent == 0 ? (seq_t)1 : (seq_t)0)
                               : big_pow((seq_t)j, (seq_t)exponent);
        if (bin_j != (seq_t)0 && pow_j != (seq_t)0)
            sum_terms += bin_j * pow_j;
    }
    return bin_ik * sum_terms;
}

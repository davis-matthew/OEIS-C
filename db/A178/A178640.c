/*
 * A178640  Triangle T(n,k) with the coefficient [x^k] of the series
 *          (-1)^(n+1) * (x-1)^(n+1) * Sum_{j>=0} (5+8*j)^n*x^j in row n,
 *          column k.
 * https://oeis.org/A178640
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where * r = floor( (sqrt(8*n + 1) - 1) / 2 ) (row
 *            index) * k = n - r*(r+1)/2 (column index) * T(r,k) =
 *            Σ_{i=0}^{min(k,r+1)} (-1)^i * C(r+1,i) * (5 + 8*(k-i))^r * * The
 *            binomial coefficients C(r+1,i) are computed multiplicatively,
 *            and * integer powers are obtained via seq_pow. All ar...
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 30, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/seq_pow.h"

static inline idx_t triangular(idx_t r)
{
    return r * (r + 1) / 2;
}

/* Return the a(n) value for the OEIS A178640 sequence.  */
seq_t a(idx_t n)
{
    /* Find the row index r such that triangular(r) <= n < triangular(r+1).  */
    idx_t lo = 0;
    idx_t hi = 1;
    while (triangular(hi) <= n)
        hi <<= 1;
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if (triangular(mid) <= n)
            lo = mid + 1;
        else
            hi = mid;
    }
    idx_t r = lo - 1;                 /* maximum r with triangular(r) <= n */
    idx_t k = n - triangular(r);      /* column index within row r */

    /* Compute T(r,k) = Σ (-1)^i * C(r+1,i) * (5 + 8*(k-i))^r  */
    idx_t max_i = (k < r + 1) ? k : (r + 1);
    seq_t result = 0;
    seq_t binom = 1;                  /* C(r+1,0) */

    for (idx_t i = 0; i <= max_i; ++i) {
        /* Base for this term */
        seq_t base = (seq_t)5 + ((seq_t)(k - i)) * (seq_t)8;

        /* Raise base to the power r */
        seq_t power = seq_pow(base, r);

        /* Term to add or subtract */
        seq_t term = binom * power;
        if (i & 1)
            result -= term;
        else
            result += term;

        /* Update binomial coefficient for next i (if any) */
        if (i < max_i) {
            seq_t numer = (seq_t)(r + 1 - i);
            binom = (binom * numer) / (seq_t)(i + 1);
        }
    }

    return result;
}

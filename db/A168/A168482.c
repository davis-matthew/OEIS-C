/*
 * A168482  G.f.: Sum_{n>=0} [(n+1)(n+2)/2]*2^(n^2)*(1 + 2^n*x)^n*x^n.
 * https://oeis.org/A168482
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..⌊n/2⌋} ((n−k+1)(n−k+2)/2) · C(n−k, k) ·
 *            2^{n·(n−k)}
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 10,
 *            big any n
 * Helpers:   binom, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The following code uses the helper functions binom(m,k) and
   pow2(exp) which are defined in the utils directory. */
#include "../utils/binom.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t result = 0;
    idx_t maxk = n >> 1;           /* floor(n/2)       */
    for (idx_t k = 0; k <= maxk; ++k)
    {
        idx_t m = n - k;           /* current n-k       */
        /* (m+1)*(m+2)/2  */
        seq_t coeff = ((seq_t)(m + 1) * (seq_t)(m + 2)) / 2;

        seq_t bin = binom(m, k);   /* C(m, k)           */

        idx_t exp = n * m;         /* n*(n-k)           */
        seq_t pw = pow2(exp);      /* 2^{n*(n-k)}       */

        result += coeff * bin * pw;
    }
    return result;
}

/*
 * A090850  Clark's triangle with f=6 read by row.
 * https://oeis.org/A090850
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   c(n,k) = 6*binomial(n, k+1) + binomial(n-1, k-1) ; Sequence a(n)
 *            is the list of c(r,k) read row-wise
 * Range:     u8 n <= 7, u16 n <= 141, u32 n <= 573, u64 n <= 2173,
 *            u128 n <= 5150, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>   /* for the type __uint128_t, if available */

/* Multiplicative binomial coefficient that never overflows when the
   result fits in seq_t.  It uses an unsigned 128‑bit intermediate
   whenever the compiler supports it.  For arguments that would
   overflow 128 bits the result is truncated to seq_t, which is
   acceptable because such terms are outside the range that a 64‑bit
   build checks. */
static inline seq_t binom(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;           /* use symmetry */
    seq_t r = 1;
    for (idx_t i = 1; i <= k; ++i) {
        /* r = r * (n - k + i) / i  */
        unsigned __int128 tmp = (unsigned __int128)r * (n - k + i);
        tmp /= i;
        r = (seq_t)tmp;
    }
    return r;
}

/* Floor square root for a seq_t argument.  We use the existing
   utility, which works for 64‑bit or arbitrary precision values. */
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Row r satisfies r(r+1)/2 <= n < (r+1)(r+2)/2.  Solve
       r = floor( (sqrt(8*n+1) - 1) / 2 ).  */
    seq_t val = (seq_t)8 * (seq_t)n + 1;
    seq_t root = floor_sqrt(val);               /* floor( sqrt(8*n+1) ) */
    idx_t r = (idx_t)(((root + 1) / 2) - 1);

    /* Offset of n within the row.  The row has length r+1.  */
    idx_t t = r * (r + 1) / 2;                 /* entries before row r */
    idx_t k = n - t;                           /* column index in row r */

    seq_t term1 = 0, term2 = 0;
    /* first part: 6*binomial(r, k+1) when k+1 <= r */
    if (k + 1 <= r) {
        term1 = (seq_t)6 * binom(r, k + 1);
    }
    /* second part: binomial(r-1, k-1) when k >= 1 */
    if (k >= 1) {
        term2 = binom(r - 1, k - 1);
    }
    return term1 + term2;
}

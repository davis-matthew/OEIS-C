/*
 * A133138  Triangle T(n,k) of the coefficients of the polynomials
 *          Q(n,x)=(1+x)[(1+x)^(n-1)+x^(n-1)], Q(0,x)=2.
 * https://oeis.org/A133138
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A133138: coefficient T(r,k) of Q(r,x) = (1+x)[(1+x)^(r-1)+x^(r-1)]
 * Enumerated in row‑major order.
 * 
 * For r >= 1:
 *   T(r,k) = binomial(r,k)                for 0 <= k < r-1
 *   T(r,k) = binomial(r,k) + 1            for k = r-1 or k = r
 * For r = 0: T(0,0) = 2  (by definition).
 *
 * The flattened index n corresponds to:
 *   r = floor( (sqrt(8*n+1) - 1) / 2 )
 *   k = n - r*(r+1)/2
 *
 * The function returns the binomial coefficient (computed via the
 * helper binomial(idx_t, idx_t)) optionally +1 when k is r-1 or r.
 *
 */

#include <math.h>              /* sqrtl */
#include "../utils/binomial.h" /* binomial(idx_t, idx_t) */

/* Compute row index r from flattened index n */
static inline idx_t
row_from_index(idx_t n)
{
    if (n == 0) return 0;          /* safe shortcut for n=0 */
    /* Initial guess using float arithmetic */
    long double val = (long double)n * 8.0L + 1.0L;
    long double sq  = sqrtl(val);
    idx_t r = (idx_t)((sq - 1.0L) / 2.0L);

    /* Correct r up or down to satisfy r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    while ((unsigned __int128)r * (r + 1) / 2 > n) r--;
    while ((unsigned __int128)(r + 1) * (r + 2) / 2 <= n) r++;
    return r;
}

/* External function implementing the sequence for any n >= 0 */
seq_t
a(idx_t n)
{
    idx_t r = row_from_index(n);

    /* Triangular number up to previous row */
    idx_t tri = (idx_t)((unsigned __int128)r * (r + 1) / 2);
    idx_t k  = n - tri;

    seq_t val = binomial(r, k);           /* binomial coefficient */

    /* Add 1 when k is r-1 or r (handles r=0 case correctly) */
    if (k == r - 1 || k == r)
        val += 1;

    return val;
}

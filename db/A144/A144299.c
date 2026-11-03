/*
 * A144299  Triangle of Bessel numbers read by rows. Row n gives T(n,n),
 *          T(n,n-1), T(n,n-2), ..., T(n,0) for n >= 0.
 * https://oeis.org/A144299
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 94, u32 n <= 216, u64 n <= 573,
 *            u128 n <= 1618, big any n
 * Validated: standard, pass, 94 terms through n = 7511 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A144299 – Triangle of Bessel numbers read by rows.
 *  a(n) = T(r,k) where
 *    r = floor( ( sqrt(8*n + 1) - 1 ) / 2 ),
 *    k = n - r*(r+1)/2,
 *    T(r,k) = 0                  if k > floor(r/2 ),
 *    T(r,k) = r! / ((r-2k)! * k! * 2^k)   otherwise.
 *  (k runs from 0 to r in each row of length r+1; entries beyond
 *   floor(r/2) are zero.) */

#include <stdint.h>
#include <math.h>

/* Compute T(n,k) according to the closed‐form formula,
 * using only integer arithmetic in seq_t. */
static inline seq_t
T_n_k(idx_t n, idx_t k)
{
    if (k > n / 2) return 0;

    /* binomial coefficient  C(n, 2k) */
    idx_t m = 2 * k;
    seq_t bin = 1;
    for (idx_t i = 1; i <= m; ++i) {
        bin = bin * (seq_t)(n - m + i);
        bin = bin / (seq_t)i;
    }

    /* odd double factorial (2k-1)!! = product_{i=1}^{k} (2i-1) */
    seq_t odd = 1;
    for (idx_t i = 1; i <= k; ++i) {
        odd = odd * (seq_t)(2 * i - 1);
    }

    return bin * odd;
}

seq_t
a(idx_t n)
{
    /* Find the row 'r' such that r*(r+1)/2 <= n < (r+1)*(r+2)/2. */
    long double dn = (long double)n;
    idx_t r = (idx_t)((sqrtl(8.0L * dn + 1.0L) - 1.0L) / 2.0L);

    /* Adjust for possible rounding errors. */
    while ((r + 1) * (r + 2) / 2 <= n) ++r;
    while (r * (r + 1) / 2 > n) --r;

    idx_t before = r * (r + 1) / 2;
    idx_t k = n - before;

    return T_n_k(r, k);
}

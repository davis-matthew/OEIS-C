/*
 * A239331  Square array, read by antidiagonals: column k has g.f.
 *          (1+(k-1)*x)^2/(1-x)^3.
 * https://oeis.org/A239331
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A239331 (OEIS):
   a(n) is obtained by reading the square array T(i,j) by antidiagonals,
   where         T(i,j) = binom(i+2,2) + 2(j-1)binom(i+1,2) + (j-1)^2 binom(i,2)
   (the coefficients of (1+(j-1)x)^2/(1-x)^3).  For a given n,
   find the diagonal d such that d(d+1)/2 <= n < (d+1)(d+2)/2,
   set i = n - d(d+1)/2 and j = d - i.  Then a(n) = T(i,j).   */

/* Note:  seq_t and idx_t are supplied by the build.  Use only integer
   arithmetic; floating‑point is employed only for the diagonal search.  */

#include <stdint.h>
#include <math.h>   /* for sqrtl */

static inline seq_t T_term(idx_t i, idx_t j)
{
    /* Compute T(i,j) using signed 128‑bit arithmetic to avoid overflow
       and to correctly handle the possible negative coefficient when j < 1. */
    __int128 ii = i;
    __int128 jj = j;

    __int128 a = (ii + 2) * (ii + 1) / 2;           /* binom(i+2,2) */
    __int128 b = (ii + 1) * ii / 2;                /* binom(i+1,2) */
    __int128 c = ii * (ii - 1) / 2;                /* binom(i,2)   */

    __int128 d = jj - 1;                           /* signed (j-1)  */

    __int128 res = a + 2 * d * b + d * d * c;
    return (seq_t)res;      /* res is non‑negative for all i,j >= 0 */
}

static inline idx_t diagonal_of(idx_t n)
{
    /* Approximate diagonal by floating point, then adjust to the exact value. */
    long double approx = sqrtl((long double)8.0L * (long double)n + 1.0L);
    idx_t d = (idx_t)((approx - 1.0L) / 2.0L);

    /* Adjust: it should satisfy d*(d+1)/2 <= n < (d+1)*(d+2)/2 */
    while (((__int128)(d + 1) * (d + 2) / 2) <= n) d++;
    while (((__int128)d * (d + 1) / 2) > n) d--;

    return d;
}

seq_t a(idx_t n)
{
    idx_t d = diagonal_of(n);                               /* diagonal */
    __int128 tri = (__int128)d * (d + 1) / 2;               /* triangular number */
    idx_t i = n - (idx_t)tri;                               /* row index in the diagonal */
    idx_t j = d - i;                                         /* column index   */
    return T_term(i, j);
}

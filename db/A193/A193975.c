/*
 * A193975  Triangular array: the self-fission of (p(n,x)), where
 *          p(n,x)=x*p(n-1,x)+n+1, where p(0,x)=1.
 * https://oeis.org/A193975
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is obtained by self‑fission of the polynomial * sequence
 *            p(n,x) with p(0,x)=1 and p(n,x)=x*p(n‑1,x)+n+1. * If n is
 *            written as n = r(r+1)/2 + k (0 ≤ k ≤ r), then * a(n) =
 *            r*(k+1)*(k+2)/2 + (3k+2) – (k-1)*k*(k-2)/6 * where r = ⌊(√(8n+1)
 *            – 1)/2⌋ and k = n – r(r+1)/2
 * Range:     u8 n <= 42, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>      /* for sqrt */

static inline idx_t isqrt_u64(idx_t n)
{
    idx_t x = (idx_t) sqrt((double) n);          /* initial estimate */
    while ((x + 1) <= n / (x + 1)) ++x;          /* adjust up if needed  */
    while (x > n / x) --x;                      /* adjust down if needed */
    return x;
}

seq_t a(idx_t n)
{
    /* Find the triangular row r and offset k such that
       n = r*(r+1)/2 + k  with 0 ≤ k ≤ r. */
    idx_t m   = 8 * n + 1;
    idx_t t   = isqrt_u64(m);                   /* floor(sqrt(m)) */
    idx_t r   = (t - 1) / 2;
    idx_t off = r * (r + 1) / 2;
    idx_t k   = n - off;

    /* Compute the components of the formula using idx_t
       to avoid overflow on very small seq_t types. */
    idx_t part1 = r * ((k + 1) * (k + 2) / 2);
    idx_t part2 = 3 * k + 2;
    idx_t sub   = (k >= 3) ? ((k - 1) * k * (k - 2) / 6) : 0;
    idx_t result = part1 + part2 - sub;

    return (seq_t) result;
}

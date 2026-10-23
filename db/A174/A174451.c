/*
 * A174451  Triangle T(n, k, q) = n!*(n+1)!*q^k/((n-k)!(n-k+1)!) if floor(n/2)
 *          > k-1, otherwise n!*(n+1)!*q^(n-k)/(k!*(k+1)!) for q = 3, read by
 *          rows.
 * https://oeis.org/A174451
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(row, col, 3) where T(r,k) =
 *            r!·(r+1)!·3^k/((r-k)!·(r-k+1)!) for k ≤ r/2, otherwise T(r,k) =
 *            r!·(r+1)!·3^(r-k)/(k!·(k+1)!). The sequence is read row‑by‑row
 *            from the triangle (offset 0)
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 59, u64 n <= 143,
 *            u128 n <= 363, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>

/* Return the row index r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2. */
static idx_t row_of(idx_t n)
{
    /* r = floor((-1 + sqrt(1 + 8*n)) / 2)  */
    long double val = sqrtl((long double)(8.0L * n + 1.0L));
    idx_t r = (idx_t)((val - 1.0L) / 2.0L);

    /* Correct possible rounding errors */
    while (((unsigned __int128)(r + 1) * (r + 2) / 2) <= n) r++;
    while (((unsigned __int128)r * (r + 1) / 2) > n) r--;
    return r;
}

seq_t a(idx_t n)
{
    /* Find row r and column k in that row */
    idx_t r = row_of(n);
    idx_t row_start = (idx_t)(((__int128)r * (r + 1)) / 2);
    idx_t k = n - row_start;          /* column index, 0‑based */

    /* Use symmetry: pick the smaller of k and r-k for the exponent. */
    idx_t m = (k <= r - k) ? k : r - k;   /* exp = min(k, r-k) */

    seq_t result = 1;
    /* Multiply successive terms: (r-i)*(r+1-i)*3  for i=0..m-1 */
    for (idx_t i = 0; i < m; ++i) {
        result *= (seq_t)(r - i);
        result *= (seq_t)(r + 1 - i);
        result *= 3;
    }
    return result;
}

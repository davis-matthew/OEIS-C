/*
 * A158860  Triangle T(n,k)= ( 1 +T(n-1,k)*T(n,k-1) ) / T(n-1,k-1) initialized
 *          by T(n,0)=3n-2, T(n,k)=0 if k>=n, read by rows 0<=k<n.
 * https://oeis.org/A158860
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) is the row‑major listing of the triangle T(n,k) with T(n,0)
 *            = 3n-2 and T(n,k) = n-k for k ≥ 1. For index n (n≥1) let r be
 *            the smallest integer with r(r+1)/2 ≥ n. Then a(n) = 3r-2 if n is
 *            the first element of row r, otherwise a(n) = r - (n - r(r-1)/2 -
 *            1)
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                    /* undefined for n=0, return 0 */

    /* Compute the row number r such that r(r-1)/2 < n <= r(r+1)/2 */
    uint64_t nn = (uint64_t)n;
    long double d = sqrt((long double)8.0*nn + 1.0);
    uint64_t r = (uint64_t)((d - 1.0L) / 2.0L);

    /* Correct r in case of rounding errors */
    while ((unsigned __int128)r * (r + 1) / 2 < nn)
        ++r;
    while (r > 0 && (unsigned __int128)(r - 1) * r / 2 >= nn)
        --r;

    /* Start index of row r */
    uint64_t start = (unsigned __int128)r * (r - 1) / 2 + 1;
    uint64_t drow = nn - start;      /* offset within the row (0‑based) */

    if (drow == 0)
        return (seq_t)(3 * r - 2);   /* first element of the row */
    else
        return (seq_t)(r - drow);    /* remaining elements */
}

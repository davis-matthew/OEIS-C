/*
 * A095890  Triangle read by rows: T(n,k) = (n-k+1)^(n-k), n>=1, 1<=k<=n.
 * https://oeis.org/A095890
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = (r - k + 1)^(r - k) where r = ceil((-1 + sqrt(1 + 8n)) /
 *            2), k = n - r*(r-1)/2
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 55, u64 n <= 136,
 *            u128 n <= 378, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* not used, offset 1 */

    /* find row r such that r(r+1)/2 >= n */
    idx_t r = (idx_t)((sqrtl((long double)(8.0L * n + 1.0L)) - 1.0L) / 2.0L);
    while ((r * (r + 1)) / 2 < n) ++r;

    /* position k in the row (1‑based) */
    idx_t k = n - (r * (r - 1)) / 2;

    /* compute (r-k+1)^(r-k) */
    seq_t base = (seq_t)(r - k + 1);
    idx_t exp = r - k;
    seq_t result = 1;

    while (exp > 0)
    {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

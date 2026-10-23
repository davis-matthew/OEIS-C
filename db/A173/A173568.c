/*
 * A173568  Triangle T(n, k) = f(k, n-k+1) + f(n-k+1, k), where f(n, k) =
 *          round( ((1+sqrt(k))^(2*n+1) - (1-sqrt(k))^(2*n+1))/(2*sqrt(k))) -
 *          1, read by rows.
 * https://oeis.org/A173568
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, less
 * Formula:   a(n) = T(r,k) where the triangle is read by rows. r =
 *            ceil((sqrt(1+8n)-1)/2) is the row number, k = n - r*(r-1)/2 is
 *            the position in that row. T(r,k) = f(k, r-k+1) + f(r-k+1, k).
 *            f(n,k) = round(((1+sqrt(k))^(2n+1) -
 *            (1-sqrt(k))^(2n+1))/(2*sqrt(k))) - 1
 * Range:     u8 n <= 6, u16 n <= 28, u32 n <= 92, u64 n <= 279, u128 n <= 825,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function below implements this definition using only integer
   arithmetic and a simple recurrence for f.  */

#include <stdint.h>

static idx_t floor_sqrt_u64(idx_t n)
{
    idx_t low = 0, high = n, ans = 0;
    while (low <= high)
    {
        idx_t mid = (low + high) >> 1;
        idx_t sq = mid * mid;
        if (sq <= n) { ans = mid; low = mid + 1; }
        else         { high = mid - 1; }
    }
    return ans;
}

/* Compute f(n,k) as defined in the OEIS entry.  n >= 0, k >= 1.  */
static seq_t compute_f(idx_t n, idx_t k)
{
    /* t_0 = 0, t_1 = 1, recurrence: t_{m+2} = 2*t_{m+1} + (k-1)*t_m  */
    seq_t t_prev2 = 0;          /* t_0 */
    seq_t t_prev1 = 1;          /* t_1 */
    if (n == 0)                 /* f(0,k) = 0 */
        return 0;

    /* We need t_{2n+1}.  Loop from m = 2 to m = 2n+1.  */
    idx_t m_max = 2 * n + 1;
    for (idx_t m = 2; m <= m_max; ++m)
    {
        seq_t t_curr = t_prev1 * (seq_t)2 + (seq_t)(k - 1) * t_prev2;
        t_prev2 = t_prev1;
        t_prev1 = t_curr;
    }
    /* t_{2n+1} is in t_prev1.  f(n,k) = t_{2n+1} - 1.  */
    return t_prev1 - (seq_t)1;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* Offset is 1, this case is unused. */

    /* Determine the row r such that (r-1)*r/2 < n <= r*(r+1)/2.  */
    idx_t disc = floor_sqrt_u64(1 + 8 * n);
    idx_t r = (disc - 1) / 2;
    if (r * (r + 1) / 2 < n) ++r;

    /* Position k in this row.  */
    idx_t k = n - (r - 1) * r / 2;

    /* Compute complementary index m = r - k + 1.  */
    idx_t m = r - k + 1;

    /* The triangle value is f(k,m) + f(m,k).  */
    seq_t term1 = compute_f(k, m);
    seq_t term2 = compute_f(m, k);
    return term1 + term2;
}

/*
 * A116399  Triangle whose k-th column has e.g.f. sum{j=0..k,
 *          Bessel_I(k+j,2x)}.
 * https://oeis.org/A116399
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle A116399: a(n) = T(row, col)
   where row = floor((sqrt(8n+1)-1)/2), col = n - row*(row+1)/2
   T(r,k) = sum_{m = ceil((r-2k)/2)}^{floor((r-k)/2)} binom(r,m)   (0‑based) */
#include <stdint.h>

 /* Find the row index r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
static inline idx_t find_row(idx_t n)
{
    idx_t low = 0, high = 1;
    while ((unsigned __int128)high * (high + 1) / 2 <= n)
        high <<= 1;                           /* double until too large   */
    while (low < high)
    {
        idx_t mid = (low + high + 1) >> 1;
        if ((unsigned __int128)mid * (mid + 1) / 2 <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* Integer binomial coefficient: binom(n,k) using only integral ops */
static seq_t my_binom(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;          /* use symmetry */
    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i)
    {
        res = res * (seq_t)(n - k + i);
        res = res / (seq_t)i;
    }
    return res;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Locate the triangle coordinates (r,k) for index n */
    idx_t r = find_row(n);
    idx_t tri = (idx_t)(((unsigned __int128)r * (r + 1)) / 2);
    idx_t k = n - tri;                 /* 0 <= k <= r */

    /* Compute bounds for summation index m */
    signed __int128 d1 = (signed __int128)r - 2 * (signed __int128)k;   /* r - 2k   */
    signed __int128 d2 = (signed __int128)r - (signed __int128)k;       /* r - k    */

    /* m_min = ceil(d1/2); integer arithmetic, truncates toward zero */
    signed __int128 m_min_signed = (d1 >= 0) ? (d1 + 1) / 2 : d1 / 2;
    idx_t m_min = (m_min_signed < 0) ? 0 : (idx_t)m_min_signed;

    /* m_max = floor(d2/2); d2 is non‑negative for allowed k */
    idx_t m_max = (idx_t)(d2 / 2);

    if (m_min > m_max) return (seq_t)0;

    seq_t result = (seq_t)0;
    for (idx_t m = m_min; m <= m_max; ++m)
        result += my_binom(r, m);

    return result;
}

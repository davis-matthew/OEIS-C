/*
 * A099614  Numerator of (1-1/n)^k - (1-k/n), 2<=k<=n, triangle read by rows.
 * https://oeis.org/A099614
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, frac
 * Formula:   a(n) = (m-1)^k - (m-k)*m^(k-1) where m is the row number (m >=
 *            2) such that the index n lies in that row, and k is the position
 *            in the row (k runs from 2 to m). The row number is determined by
 *            solving n <= m(m-1)/2
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

/* Binary exponentiation for seq_t (works for built‑in types
   and for arbitrary‑precision integer classes). */
static seq_t seq_pow(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp > 0)
    {
        if (exp & (idx_t)1) result *= base;
        base *= base;
        exp >>= (idx_t)1;
    }
    return result;
}

/* Find the smallest m such that m(m-1)/2 >= idx.
   Uses a simple binary search – safe for all idx_t sizes. */
static idx_t find_row(idx_t idx)
{
    idx_t lo = (idx_t)2;
    idx_t hi = (idx_t)2;
    while ((hi * (hi - (idx_t)1)) / (idx_t)2 < idx)
        hi <<= (idx_t)1;          /* hi *= 2 */

    while (lo < hi)
    {
        idx_t mid = lo + (hi - lo) / (idx_t)2;
        idx_t cum = (mid * (mid - (idx_t)1)) / (idx_t)2;
        if (cum >= idx)
            hi = mid;
        else
            lo = mid + (idx_t)1;
    }
    return lo;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < (idx_t)1) return (seq_t)0;   /* undefined for n == 0 */

    /* Determine the triangle row (m) and the position (k). */
    idx_t m = find_row(n);
    idx_t cum_before = ((m - (idx_t)1) * (m - (idx_t)2)) / (idx_t)2;
    idx_t pos0 = n - cum_before - (idx_t)1;      /* 0‑based position in row */
    idx_t k   = pos0 + (idx_t)2;                 /* k ranges from 2 to m */

    /* Compute the numerator (m-1)^k - (m-k)*m^(k-1). */
    seq_t pow_mminus1_k = seq_pow((seq_t)(m - (idx_t)1), k);
    seq_t pow_m_kminus1 = seq_pow((seq_t)m, k - (idx_t)1);
    seq_t factor = (seq_t)(m - k);
    seq_t term2 = factor * pow_m_kminus1;
    seq_t result = pow_mminus1_k - term2;

    return result;
}

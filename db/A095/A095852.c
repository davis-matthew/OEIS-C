/*
 * A095852  Triangle read by rows: T(n,k) = (n-k+1)^(k^2), n>=1, 1<=k<=n.
 * https://oeis.org/A095852
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) is the (n‑th) entry of the triangle T(r,k) = (r - k +
 *            1)^(k^2) read row by row. For a given n (1‑based), find the row
 *            r such that r*(r+1)/2 ≥ n and prev = (r-1)*r/2; then k = n -
 *            prev. Result = (r - k + 1)^(k*k)
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 24, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Convert an integer exponent to seq_t using binary exponentiation. */
static seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp)
    {
        if (exp & (idx_t)1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

/* Find the row r (1‑based) that contains the n‑th term. */
static idx_t find_row(idx_t n)
{
    if (n <= 1) return (idx_t)1;
    idx_t lo = (idx_t)1, hi = (idx_t)1;
    /* Double hi until the triangular number exceeds n. */
    while (1)
    {
        __int128 tri = (__int128)hi * (hi + (idx_t)1) / 2;
        if (tri >= n)
            break;
        hi <<= 1;
        if (hi == 0)  /* overflow, stop at maximum value */
        {
            hi = n;    /* worst‑case upper bound */
            break;
        }
    }
    /* Binary search between lo and hi. */
    while (lo < hi)
    {
        idx_t mid = lo + (hi - lo) / 2;
        __int128 tri = (__int128)mid * (mid + (idx_t)1) / 2;
        if (tri >= n)
            hi = mid;
        else
            lo = mid + (idx_t)1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* index below offset is undefined */

    idx_t row = find_row(n);

    __int128 prev_tri = (__int128)(row - (idx_t)1) * row / 2;
    idx_t k = n - (idx_t)prev_tri;    /* column, 1‑based, 1 ≤ k ≤ row */

    idx_t base_val = row - k + (idx_t)1;          /* (r - k + 1) */
    idx_t exp_val  = k * k;                       /* k^2 */

    return pow_seq_t((seq_t)base_val, exp_val);
}

/*
 * A095838  Triangle read by rows: T(n,k) = n^((n-k+1)^2), n>=1, 1<=k<=n.
 * https://oeis.org/A095838
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = r^((r-k+1)^2) where r is the row such that r(r+1)/2 ≥ n
 *            and k = n - r(r-1)/2. This is the triangle read by rows for
 *            T(n,k) = n^((n-k+1)^2)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 6, u64 n <= 15, u128 n <= 21,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"   /* seq_pow(seq_t base, idx_t exp) */

/* Find the row r containing the n-th term, using binary search on triangular
 * numbers.  This avoids floating‑point arithmetic. */
static idx_t find_row(idx_t n)
{
    idx_t low = 1;
    idx_t high = n;          /* r never exceeds n */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        __int128 tri = (__int128)mid * (mid + 1) / 2;
        if (tri >= (__int128)n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n == 0)          /* sequence is offset 1; undefined for n==0 */
        return (seq_t)0;

    /* Determine the row (r) and column (k) of the n‑th term. */
    idx_t r = find_row(n);
    __int128 prev_tri = (__int128)(r - 1) * r / 2;
    idx_t k = (idx_t)(n - prev_tri);   /* 1 ≤ k ≤ r */

    /* Compute the exponent: (r - k + 1)^2. */
    idx_t p = r - k + 1;
    idx_t exp = p * p;

    /* Compute r^exp using the generic seq_pow helper. */
    return seq_pow((seq_t)r, exp);
}

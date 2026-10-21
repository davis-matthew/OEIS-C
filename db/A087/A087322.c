/*
 * A087322  Triangle T read by rows: T(n, 1) = 2*n + 1. For 1 < k <= n, T(n,
 *          k) = 2*T(n,k-1) + 1.
 * https://oeis.org/A087322
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = (r+1) * 2^k – 1, where r = smallest integer with r(r+1)/2
 *            ≥ n (row number), and k = n – r(r–1)/2 (column number)
 * Range:     u8 n <= 20, u16 n <= 90, u32 n <= 405, u64 n <= 1769,
 *            u128 n <= 7502, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* not used, just a safety net */

    /*--- find r such that r(r+1)/2 ≥ n -------------------------------*/
    idx_t lo = 0, hi = 1;
    while ((__int128)hi * (hi + 1) / 2 < n) hi <<= 1;   /* grow hi until condition met */

    while (lo + 1 < hi) {                   /* binary search */
        idx_t mid = lo + (hi - lo) / 2;
        if ((__int128)mid * (mid + 1) / 2 >= n)
            hi = mid;
        else
            lo = mid;
    }
    idx_t r = hi;                            /* row number (1‑based) */

    /*--- column index -----------------------------------------------*/
    idx_t start = r * (r - 1) / 2;           /* t(r–1) */
    idx_t k = n - start;                     /* 1‑based column */

    /*--- compute (r+1) * 2^k – 1 -------------------------------*/
    seq_t val = (seq_t)(r + 1);               /* start with (r+1) */
    for (idx_t i = 0; i < k; ++i) val *= 2;   /* multiply by 2, k times */
    val -= 1;                                 /* subtract 1 */

    return val;
}

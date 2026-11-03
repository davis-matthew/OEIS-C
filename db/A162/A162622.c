/*
 * A162622  Triangle read by rows in which row n lists n+1 terms, starting
 *          with n, such that the difference between successive terms is equal
 *          to n^4 - 1.
 * https://oeis.org/A162622
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = r + k*(r^4-1), where r = floor((-1+sqrt(1+8n))/2), k = n
 *            - r*(r+1)/2 (r is the row, k the position within that row)
 * Range:     u8 n <= 10, u16 n <= 61, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 64 terms through n = 944 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t row_from_offset(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo < hi)
    {
        idx_t mid = lo + (hi - lo + 1) / 2;
        __int128 val = (__int128)mid * (mid + 1) / 2;
        if (val <= (__int128)n)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    idx_t r = row_from_offset(n);
    idx_t row_start = r * (r + 1) / 2;
    idx_t k = n - row_start;

    seq_t rseq = (seq_t)r;
    seq_t r2   = rseq * rseq;          /* r^2   */
    seq_t r4   = r2 * r2;              /* r^4   */
    seq_t diff = r4 - (seq_t)1;        /* r^4-1 */
    seq_t term = rseq + (seq_t)k * diff;

    return term;
}

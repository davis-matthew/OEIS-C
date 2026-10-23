/*
 * A122774  Triangle of bifactorial numbers, n B m = (2(n-m)-1)!! (2(n-1))!! /
 *          (2(n-m))!!, read by rows.
 * https://oeis.org/A122774
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = B(r,c), where r is the smallest integer with * r(r+1)/2
 *            >= n, c = n - r(r-1)/2. * B(r,c) = (2(r-c)-1)!! * (2(r-1))!! /
 *            (2(r-c))!!
 * Range:     u8 n <= 14, u16 n <= 28, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS sequence A122774. */
#include <stdint.h>

static inline idx_t isqrt_idx(idx_t x)
{
    /* integer square root of x: largest y with y*y <= x */
    idx_t lo = 0, hi = ((idx_t)1ULL << 32);  /* sqrt(2^64) <= 2^32 */
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    /* find row r */
    idx_t t = 8 * n + 1;
    idx_t s = isqrt_idx(t);
    idx_t r = (s - 1) / 2;
    if (r * (r + 1) / 2 < n)
        r++;
    /* column within the row (1‑based) */
    idx_t prev = (r - 1) * r / 2;
    idx_t c = n - prev;

    /* compute B(r,1) = product of odd numbers up to (2*(r-1)-1) */
    seq_t val = 1;
    for (idx_t k = 1; k < r; ++k)
        val *= (seq_t)(2 * k - 1);

    if (c == 1)
        return val;

    /* advance within the row using the recurrence
       B(r,m+1) = B(r,m) * 2*(r-m) / (2*(r-m)-1)  */
    for (idx_t m = 1; m < c; ++m) {
        idx_t mult = 2 * (r - m);
        idx_t den  = 2 * (r - m) - 1;
        val = val * (seq_t)mult / (seq_t)den;
    }
    return val;
}

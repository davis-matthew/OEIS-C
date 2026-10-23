/*
 * A133825  Triangle whose rows are sequences of increasing and decreasing
 *          triangular numbers: 1; 1,3,1; 1,3,6,3,1; ... .
 * https://oeis.org/A133825
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   a(n) = T(k) where k = 1 + c (c < r) = 2r-1 - c (c ≥ r) r =
 *            floor(sqrt(n)) + 1 c = n - (r-1)^2 T(k) = k*(k+1)/2 with 0‑based
 *            index n
 * Range:     u8 n <= 505, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square‑root: floor(sqrt(n)) for unsigned 64‑bit n */
static uint64_t floor_sqrt_u64(uint64_t n)
{
    if (n < 2) return n;           /* 0 or 1 */

    uint64_t lo = 0, hi = n >> 1;  /* hi <= sqrt(n) for n>=2 */
    while (lo < hi)
    {
        uint64_t mid = (lo + hi + 1) >> 1;   /* upper mid */
        uint64_t div = n / mid;
        if (mid > div) /* mid*mid > n without overflow */
            hi = mid - 1;
        else
            lo = mid;
    }
    return lo;
}

seq_t a(idx_t n)
{
    uint64_t r = floor_sqrt_u64((uint64_t)n) + 1;   /* 1‑based row number */
    uint64_t base = (r - 1) * (r - 1);              /* first index of row r */
    uint64_t c = n - base;                          /* column offset in row */

    uint64_t k;
    if (c < r)
        k = c + 1;                                 /* ascending part */
    else
        k = (2 * r - 1) - c;                       /* descending part */

    seq_t kk = (seq_t)k;
    return kk * (kk + 1) / 2;                       /* triangular number */
}

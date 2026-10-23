/*
 * A046902  Clark's triangle: left border = 0 1 1 1..., right border =
 *          multiples of 6; other entries = sum of 2 entries above.
 * https://oeis.org/A046902
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl, nice
 * Formula:   a(n) is the element of Clark's triangle read row–by–row.
 *            Triangle entry T(n,k) = 6*binomial(n,k-1) + binomial(n-1,k) with
 *            T(0,0)=0. The sequence a is defined by a(0)=0, a(1)=1, a(2)=6,
 *            a(3)=1, a(4)=7, a(5)=12, …, as produced by reading the triangle
 *            left to right, row by row
 * Range:     u8 n <= 39, u16 n <= 143, u32 n <= 575, u64 n <= 2175,
 *            u128 n <= 8574, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

/* integer floor square root for unsigned 64-bit values ( only needed for
   n up to about 10 000, so overflow in mid*mid is impossible ) */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        idx_t sq = mid * mid;
        if (sq == n) return mid;
        if (sq < n) {
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return hi;
}

/* compute binomial coefficient C(n,k) using gcd‑based cancellation to avoid
   overflow for 64‑bit seq_t values.  The routine works for any integer
   seq_t type that supports +, -, *, /, comparisons and gcd(seq_t,seq_t). */
static seq_t binom(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k == 0 || k == n) return 1;

    if (k > n - k) k = n - k;            /* use symmetry */

    seq_t result = 1;
    for (idx_t i = 1; i <= k; ++i) {
        seq_t numer = (seq_t)(n - k + i);
        seq_t denom = (seq_t)i;

        /* cancel common factors between numer and denom */
        seq_t g = gcd(numer, denom);
        numer /= g;
        denom /= g;

        /* cancel common factors between result and remaining denom */
        g = gcd(result, denom);
        result /= g;
        denom /= g;

        /* after cancellations denom should divide numer exactly,
           but we multiply first keeping intermediate small */
        result *= numer;
        /* denom should be 1 here; ignore the remainder if any */
    }
    return result;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;       /* initial term */

    /* find row r such that r*(r+1)/2 < n <= (r+1)*(r+2)/2 */
    idx_t tmp = 8 * n + 1;
    idx_t s   = isqrt_u64(tmp);
    idx_t r   = (s - 1) >> 1;

    idx_t prev = r * (r + 1) / 2;       /* number of elements before row r */
    idx_t c    = n - prev;              /* column in the current row (0‑based) */

    seq_t bin1 = 0;
    if (c > 0)
        bin1 = binom(r, c - 1);

    seq_t bin2 = 0;
    if (r > 0)
        bin2 = binom(r - 1, c);

    return 6 * bin1 + bin2;
}

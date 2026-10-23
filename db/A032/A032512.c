/*
 * A032512  Sum of the integer part of 4th roots of integers <= n.
 * https://oeis.org/A032512
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A032512
   Formula: a(n) = Σ_{i=1..n} ⌊i^{1/4}⌋
   Computation: use the constant‑block formula based on the integer
   fourth root of n.  The routine is O( t ) where t = ⌊n^{1/4}⌋
   (≤ 65535 for uint64_t n).  Only integer arithmetic is used. */

#include <stdint.h>

/* Return floor of the fourth root of x, 0 ≤ x < 2^64.
   Binary search on the range [0, 65536].  The search uses
   128‑bit arithmetic to avoid overflow of x⁴. */
static idx_t floor_fourth_root(idx_t x)
{
    if (x < 1)
        return 0;

    idx_t low = 0, high = 65537;          /* 65536 is the maximum possible root,
                                            we search in [0,65536] inclusive.      */
    while (low < high) {
        idx_t mid      = (low + high) >> 1;
        unsigned __int128 mid2 = (unsigned __int128)mid * mid;
        unsigned __int128 mid4 = mid2 * mid2;

        if (mid4 <= (unsigned __int128)x)
            low = mid + 1;
        else
            high = mid;
    }
    return low - 1;                       /* last successful mid */
}

/* Sum of the integer part of fourth roots of integers ≤ n.
   a(0) = 0.  The summation uses the block formula
   a(n) = Σ_{j=1}^{t-1} j·[(j+1)^4 - j^4] + t·[n - t^4 + 1],
   where t = ⌊n^{1/4}⌋.  All intermediate quantities fit
   into 64 bits for uint64_t inputs. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    idx_t t = floor_fourth_root(n);          /* t = ⌊n^{1/4}⌋ */
    seq_t sum = 0;

    /* contributions from 1 … t-1 */
    for (idx_t j = 1; j < t; ++j) {
        idx_t j2  = j * j;
        idx_t j4  = j2 * j2;
        idx_t jp1 = j + 1;
        idx_t jp12 = jp1 * jp1;
        idx_t jp14 = jp12 * jp12;
        idx_t cnt = jp14 - j4;                /* number of i with floor(i^{1/4}) = j */
        sum += (seq_t)j * (seq_t)cnt;
    }

    /* contribution from the last block (t) */
    idx_t t2   = t * t;
    idx_t t4   = t2 * t2;
    idx_t cnt_last = n - t4 + 1;               /* numbers with floor = t  */
    sum += (seq_t)t * (seq_t)cnt_last;

    return sum;
}

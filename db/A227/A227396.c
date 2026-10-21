/*
 * A227396  Triangle A074909(n) with the first column equal to 1 followed by
 *          -A000027(n) instead of A000012.
 * https://oeis.org/A227396
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) is the n-th number in the flattened triangle A074909. * Row
 *            r (r>=0) has r+1 entries: * if r==0: a(0)=1 * else: first entry
 *            is -r, * each following entry (k>=1) is C(r+1, k). * The
 *            sequence is a(0)=1, a(1)=-1, a(2)=2, a(3)=-2, a(4)=3, a(5)=3, …
 * Range:     i8 n <= 30, i16 n <= 55, i32 n <= 55, i64 n <= 55, i128 n <= 55,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

static idx_t row_of_n(idx_t n)
{
    /* Find the largest r such that r*(r+1)/2 <= n. */
    if (n == 0) return 0;
    idx_t lo = 0;         /* r* (r+1)/2 <= n holds for lo */
    idx_t hi = 1;         /* r* (r+1)/2 > n holds for hi; 1 is safe for n>0 */
    while (((unsigned __int128)hi * (hi + 1) / 2) <= n)
    {
        hi <<= 1;
        if (hi == 0) break;   /* overflow guard; impossible for 64‑bit n */
    }
    while (lo + 1 < hi)
    {
        idx_t mid = lo + (hi - lo) / 2;
        if (((unsigned __int128)mid * (mid + 1) / 2) <= n)
            lo = mid;
        else
            hi = mid;
    }
    return lo;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;          /* a(0)=1 */
    idx_t r = row_of_n(n);
    idx_t start = (idx_t)((unsigned __int128)r * (r + 1) / 2);
    idx_t k = n - start;                   /* column in the row (0-based) */
    if (k == 0)                            /* first element of the row */
        return -(seq_t)r;                  /* -r for r>0 */
    return binomial(r + 1, k);             /* other entries */
}

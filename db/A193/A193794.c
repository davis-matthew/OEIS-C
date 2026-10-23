/*
 * A193794  Triangular array: the fusion of polynomial sequences P and Q given
 *          by p(n,x)=(3x+1)^n and q(n,x)=1+x^n.
 * https://oeis.org/A193794
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   Let r be the largest integer such that r*(r+1)/2 <= n Let c = n
 *            - r*(r+1)/2 ( 0 <= c <= r ) Then if r == 0 : a(n) = 1 else if c
 *            == r : a(n) = 4^(r-1) else : a(n) = binom(r-1, c) * 3^c All
 *            operations are performed in type seq_t
 * Range:     u8 n <= 19, u16 n <= 53, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   binom, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <limits.h>
#include "../utils/binom.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* Find the row r:  r*(r+1)/2 <= n < (r+1)*(r+2)/2   */
    idx_t lo = 0;
    idx_t hi = 1;

    /* Grow hi until the triangular number exceeds n.
       The loop is safe for all 0 <= n <= UINT64_MAX. */
    while (1) {
        unsigned __int128 t = (unsigned __int128)hi * (hi + 1) / 2;
        if (t > n) break;
        if (hi > (UINT64_MAX >> 1)) break; /* avoid shift overflow   */
        hi <<= 1;
    }

    /* Binary search between lo and hi to get the exact r  */
    while (lo + 1 < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        unsigned __int128 t = (unsigned __int128)mid * (mid + 1) / 2;
        if (t <= n)
            lo = mid;
        else
            hi = mid;
    }
    idx_t r = lo;

    /* Compute the starting index of this row and column offset   */
    unsigned __int128 start128 = (unsigned __int128)r * (r + 1) / 2;
    idx_t start = (idx_t)start128;
    idx_t c = n - start;          /* 0 <= c <= r */

    /* Row 0 is a special constant value 1 */
    if (r == 0)
        return (seq_t)1;

    /* Last element of the row  (c == r) */
    if (c == r) {
        /* 4^(r-1)  */
        return seq_pow((seq_t)4, r - 1);
    }

    /* General element: binomial coefficient times a power of 3   */
    seq_t pow3 = seq_pow((seq_t)3, c);
    seq_t bin   = binom(r - 1, c);
    return pow3 * bin;
}

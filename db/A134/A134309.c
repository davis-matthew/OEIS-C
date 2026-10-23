/*
 * A134309  Triangle read by rows, where row n consists of n zeros followed by
 *          2^(n-1).
 * https://oeis.org/A134309
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 0 unless 9+8n is a perfect square s with (s-3) even; if
 *            such s exists, let m = (s-3)/2. Then a(n) = 1 if m==0, else
 *            2^(m-1)
 * Range:     u8 n <= 53, u16 n <= 97, u32 n <= 97, u64 n <= 97, u128 n <= 97,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

static inline idx_t isqrt_u64(idx_t x)
{
    /* integer square root: largest r such that r*r <= x */
    idx_t low = 0, high = x;
    /* Handle x==0 explicitly to avoid division by zero in the loop */
    if (x == 0) return 0;
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;   /* ceil midpoint */
        if (mid <= x / mid)                  /* mid*mid <= x without overflow */
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* Compute t = 9 + 8*n */
    idx_t t = 9 + 8 * n;

    /* Integer sqrt of t */
    idx_t s = isqrt_u64(t);
    if (s * s != t)          /* not a perfect square -> zero */
        return 0;

    /* Check that s-3 is even; this ensures m integer */
    if (s == 3)               /* m = 0 case */
        return 1;
    if ((s - 3) & 1)          /* odd -> no integer m */
        return 0;

    idx_t m = (s - 3) / 2;    /* m >= 1 here */
    /* a(n) = 2^(m-1) */
    return pow2(m - 1);
}

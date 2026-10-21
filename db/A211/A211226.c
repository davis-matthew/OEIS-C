/*
 * A211226  Triangular array: T(n,k) = f(n)/(f(k)*f(n-k)), where f(n) =
 *          (floor(n/2))!.
 * https://oeis.org/A211226
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = T(r,k) where the sequence is the triangle read by rows.
 *            T(r,k) = f(r)/(f(k)*f(r-k)) with f(m) = (⌊m/2⌋)!. The mapping
 *            from the flat index n to the triangle coordinates is: let r be
 *            the largest integer with r(r+1)/2 ≤ n; then k = n - r(r+1)/2.
 *            The value is: if r = 2m: if k even: T = binomia...
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   binomial, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/mul_small.h"

/* Find row r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2.  */
static idx_t find_row(idx_t n)
{
    idx_t low = 0, high = 1, mid;
    /* Grow high until it exceeds the required range. */
    while ((unsigned __int128)high * (high + 1) / 2 <= n) {
        high <<= 1;
        if (high == 0) break;          /* overflow guard */
    }
    /* Binary search for the largest r with r*(r+1)/2 ≤ n. */
    while (low <= high) {
        mid = low + (high - low) / 2;
        unsigned __int128 val = (unsigned __int128)mid * (mid + 1) / 2;
        if (val <= n) {
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return high;
}

seq_t a(idx_t n)
{
    idx_t r = find_row(n);
    idx_t start = (idx_t)((unsigned __int128)r * (r + 1) / 2);
    idx_t k = n - start;

    if ((r & 1) == 0) {               /* r = 2*m */
        idx_t m = r >> 1;              /* m = r/2 */
        if ((k & 1) == 0) {           /* k even -> T = binomial(m, k/2) */
            return binomial(m, k >> 1);
        } else {                      /* k odd  -> T = m * binomial(m-1, (k-1)/2) */
            return mul_small(binomial(m - 1, k >> 1), (unsigned int)m);
        }
    } else {                           /* r = 2*m+1 */
        idx_t m = r >> 1;              /* floor(r/2) = m */
        return binomial(m, k >> 1);
    }
}

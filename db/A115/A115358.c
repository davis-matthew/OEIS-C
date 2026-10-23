/*
 * A115358  Inverse of matrix (1,x)+(x,x^2) (expressed in Riordan array
 *          notation).
 * https://oeis.org/A115358
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 123, i16 n <= 123, i32 n <= 123, i64 n <= 123,
 *            i128 n <= 123, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A115358 – Inverse of matrix (1,x)+(x,x^2) (Riordan array notation).
 *
 * The triangle is the inverse of the lower‐triangular matrix whose entries are
 *   a(r,k) = 1   if r = k
 *   a(r,k) = 1   if r = 2k + 1
 *   a(r,k) = 0   otherwise
 *
 * Its inverse has a very simple closed form:
 *   inv(r,k) = (-1)^t   if  r + 1 = 2^t · (k + 1)
 *   inv(r,k) =   0      otherwise
 *  
 * The sequence a(n) is obtained by reading this triangle row‑wise (left to right)
 * and enumerating the entries with the 0‑based index n.
 */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find the row r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2.
       Binary search with 128‑bit arithmetic to avoid overflow. */
    idx_t low  = 0;
    idx_t high = n + 1;          /* high is exclusive and safe */
    while (low + 1 < high) {
        idx_t mid = low + (high - low) / 2;
        __int128 tri = (__int128)mid * (mid + 1) / 2;
        if (tri <= n)
            low = mid;
        else
            high = mid;
    }
    idx_t r = low;
    idx_t offset = (idx_t)((__int128)r * (r + 1) / 2);
    idx_t k = n - offset;

    /* Compute the value of the inverse matrix entry at (r,k). */
    idx_t r1 = r + 1;
    idx_t k1 = k + 1;
    if (k1 == 0)          /* should never happen */
        return 0;

    if (r1 % k1 != 0)     /* no solution to r+1 = 2^t (k+1) */
        return 0;

    idx_t q = r1 / k1;    /* q must be a power of two   */
    if ((q & (q - 1)) != 0)
        return 0;

    /* t = log2(q) is the number of trailing zeros of q. */
    int t = 0;
    while (q > 1) {
        q >>= 1;
        ++t;
    }

    return (seq_t)((t & 1) ? -1 : 1);
}

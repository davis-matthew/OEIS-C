/*
 * A094414  Triangle T read by rows: dot product <1,2,...,r> *
 *          <s+1,s+2,...,r,1,2,...,s>.
 * https://oeis.org/A094414
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,s) where r = floor((sqrt(8*n+1)+1)/2), s = n -
 *            r*(r-1)/2, and T(r,s) = r*(2*r^2 + 3*r - 3*r*s + 1 + 3*s^2)/6
 *            (with r >= 1, 0 <= s <= r-1). This reproduces the triangle 1; 5,
 *            4; 14, 11, 11;
 * Range:     u8 n <= 35, u16 n <= 1652, u32 n <= 5049, u64 n <= 5049,
 *            u128 n <= 5049, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root (floor) for 64‑bit unsigned numbers.
   Returns the largest r such that r*r <= x. */
static seq_t isqrt_uint64(uint64_t x)
{
    uint64_t low = 0, high = x;
    while (low <= high)
    {
        uint64_t mid = low + (high - low) / 2;
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == x)
            return (seq_t)mid;
        if (sq < x)
            low = mid + 1;
        else
        {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return (seq_t)high;
}

/* Compute the n-th term of sequence A094414. */
seq_t a(idx_t n)
{
    /* Step 1: find r such that r*(r-1)/2 <= n < r*(r+1)/2 */
    uint64_t t = 8 * (uint64_t)n + 1;          /* t <= 8*n+1 */
    seq_t root = isqrt_uint64(t);              /* floor(sqrt(t)) */
    seq_t r = (root + 1) / 2;                  /* r >= 1 */

    /* Step 2: compute s = n - r*(r-1)/2 */
    seq_t offset = (r * (r - 1)) / 2;
    seq_t s = (seq_t)n - offset;

    /* Step 3: evaluate T(r,s) */
    seq_t rr = r;
    seq_t ss = s;
    seq_t inner = 2 * rr * rr + 3 * rr - 3 * rr * ss + 1 + 3 * ss * ss;
    seq_t result = rr * inner / 6;
    return result;
}

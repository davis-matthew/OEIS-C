/*
 * A186146  Rank of n^3 when {i^2: i>=1} and {j^3: j>=1} are jointly ranked
 *          with i^2 before j^3 when i^2=j^3. Complement of A186145.
 * https://oeis.org/A186146
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = n + floor(n^(3/2))
 * Range:     u8 n <= 36, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS sequence A186146. */
#include <stdint.h>

/* Integer square root for unsigned __int128 using binary search.
 * Returns the greatest s such that s*s <= x. */
static inline unsigned __int128
isqrt128(unsigned __int128 x)
{
    if (x == 0) return 0;
    unsigned __int128 lo = 0;
    unsigned __int128 hi = 1;
    /* Find an upper bound hi such that hi*hi > x. */
    while (hi <= x / hi) hi <<= 1;
    /* Binary search between lo (hi >> 1) and hi-1. */
    lo = hi >> 1;
    while (lo <= hi) {
        unsigned __int128 mid = lo + ((hi - lo) >> 1);
        unsigned __int128 div = x / mid;
        if (mid <= div)  /* mid*mid <= x */
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;
}

seq_t a(idx_t n)
{
    /* Use 128‑bit arithmetic for intermediate computations
       to avoid overflow even when seq_t is small. */
    unsigned __int128 N   = (unsigned __int128)n;
    unsigned __int128 n3  = N * N * N;          /* n^3 */
    unsigned __int128 sq  = isqrt128(n3);       /* floor(sqrt(n^3)) */
    unsigned __int128 res = N + sq;             /* n + floor(n^(3/2)) */
    return (seq_t)res;
}

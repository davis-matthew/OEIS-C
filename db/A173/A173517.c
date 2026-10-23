/*
 * A173517  a(n) = k if n is the k-th nonsquare, zero otherwise.
 * https://oeis.org/A173517
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n - floor(sqrt(n)) for non‑squares, 0 for squares
 * Range:     u8 n <= 271, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Integer square root for unsigned 64‑bit values. */
static idx_t isqrt_u64(idx_t n)
{
    if (n < 2ULL)
        return n;

    idx_t low  = 0ULL;
    idx_t high = n < 0xFFFFFFFFULL ? n : 0xFFFFFFFFULL; /* sqrt(2^64-1) < 2^32 */

    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        if (mid > 0 && mid > n / mid) {
            high = mid - 1;
        } else {
            /* mid*mid <= n ; check if (mid+1)^2 > n */
            if ((mid + 1ULL) > n / (mid + 1ULL))
                return mid;
            low = mid + 1ULL;
        }
    }
    return high;
}

seq_t a(idx_t n)
{
    if (n < 2ULL)          /* 0 and 1 are perfect squares */
        return 0;

    idx_t r = isqrt_u64(n);
    if (r * r == n)        /* perfect square */
        return 0;

    return (seq_t)(n - r); /* rank among non‑squares */
}

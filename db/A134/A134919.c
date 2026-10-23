/*
 * A134919  Floor(n^(5/3)).
 * https://oeis.org/A134919
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n^(5/3))
 * Range:     u8 n <= 27, u16 n <= 776, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute the floor of the fifth power of n, then its integer cube root. */
seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;                  /* behaviour for indices below the offset */
    }

    /* Convert index to a 128‑bit type to avoid overflow when raising to the 5th power. */
    __int128 n128 = (__int128)n;
    __int128 n5 = n128 * n128 * n128 * n128 * n128;   /* n^5 */

    /* Binary search for the largest integer c such that c^3 <= n^5. */
    idx_t low  = 0;
    /* For the expected n range, n*n fits in idx_t. */
    idx_t high = n * n;

    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        __int128 mid3 = (__int128)mid * mid * mid;
        if (mid3 <= n5) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    return (seq_t)high;   /* high is the integer cube root of n^5 */
}

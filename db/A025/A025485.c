/*
 * A025485  Number of iterations of function f(k) = ceiling(sqrt(k))^2 - k on
 *          n required to force n <= 2.
 * https://oeis.org/A025485
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A025485
   a(n) = number of iterations of f(k) = ceil(sqrt(k))^2 - k needed to
   reduce n to <= 2.
*/
#include <stdint.h>
#include <stdbool.h>

/* integer ceil‑square‑root for a 64‑bit unsigned integer */
static inline idx_t ceil_sqrt(idx_t n)
{
    if (n == 0) return 0;
    idx_t low = 1;
    idx_t high = (idx_t)1 << 32;   /* sqrt(2^64−1) < 2^32 */
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq < n)
            low = mid + 1;
        else
            high = mid;
    }
    return low;   /* low*low >= n  and  (low-1)^2 < n */
}

seq_t a(idx_t n)
{
    seq_t steps = 0;
    idx_t k = n;
    while (k > 2) {
        idx_t m = ceil_sqrt(k);                 /* ceil(sqrt(k))          */
        idx_t diff = (idx_t)((unsigned __int128)m * m - k);  /* f(k)                 */
        k = diff;
        ++steps;
    }
    return steps;
}

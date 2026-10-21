/*
 * A091448  Numbers k such that a1 > a2, where [a0,a1,a2,...] is the simple
 *          continued fraction of sqrt(k).
 * https://oeis.org/A091448
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 42, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A091448: numbers k such that in the simple continued fraction of sqrt(k),
   the first partial quotient a1 exceeds the second (a1 > a2).
   This function returns the nth term (n ≥ 1). */

#include <stdint.h>

/* integer square root for uint64_t */
static idx_t isqrt_u64(idx_t n)
{
    if (n < 2) return n;
    idx_t low = 1, high = (n >> 1) + 1;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == n) return mid;
        if (sq < n)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

/* Return the nth member of sequence A091448 (1‑based). */
seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t k = 2;; ++k) {
        idx_t a0 = isqrt_u64(k);
        if (a0 * a0 == k) continue;               /* skip perfect squares */

        /* compute first two partial quotients a1, a2 */
        idx_t d1 = k - (unsigned __int128)a0 * a0;   /* d1 > 0 */
        idx_t a1 = (unsigned __int128)(2 * a0) / d1;  /* floor(2*a0/d1) */
        idx_t m1 = a0;                               /* m1 = a0 */

        idx_t m2 = d1 * a1 - m1;                     /* m2 = d1*a1 - m1 */
        idx_t d2 = (k - (unsigned __int128)m2 * m2) / d1;
        idx_t a2 = (a0 + m2) / d2;                   /* floor((a0+m2)/d2) */

        if (a1 > a2) {
            ++count;
            if (count == n) {
                return (seq_t)k;
            }
        }
    }
}

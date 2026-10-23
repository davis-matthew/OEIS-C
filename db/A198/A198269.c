/*
 * A198269  Ceiling(n*sqrt(12)).
 * https://oeis.org/A198269
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(n*sqrt(12))
 * Range:     u8 n <= 2, u16 n <= 15, u32 n <= 2851, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline seq_t isqrt_floor(seq_t v)
{
    seq_t low = 0, high = v;
    while (low <= high) {
        seq_t mid = low + ((high - low) / 2);
        seq_t sq = mid * mid;
        if (sq <= v)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* a(0) = 0 */
    seq_t nn = (seq_t)n;
    seq_t x  = nn * nn * 12;    /* x = 12*n^2   */
    seq_t s  = isqrt_floor(x);  /* floor(sqrt(12*n^2)) */
    return s + 1;               /* ceil(...)   */
}

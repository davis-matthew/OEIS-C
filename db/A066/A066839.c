/*
 * A066839  a(n) = sum of positive divisors k of n with k <= sqrt(n).
 * https://oeis.org/A066839
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of positive divisors k of n with k ≤ sqrt(n)
 * Range:     u8 n <= 1679, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root of an idx_t value (floor(sqrt(x))) */
static inline idx_t isqrt_idx(idx_t x)
{
    idx_t lo = 0;
    idx_t hi = x;
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;          /* ceil */
        if ((unsigned __int128)mid * mid <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                      /* safety for n < 1 */
    idx_t limit = isqrt_idx(n);
    seq_t sum = 0;
    for (idx_t i = 1; i <= limit; ++i) {
        if (n % i == 0)
            sum += (seq_t)i;
    }
    return sum;
}

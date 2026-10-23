/*
 * A130829  2n+1 appears 2n times.
 * https://oeis.org/A130829
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*floor(sqrt(n)+1/2)+1
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute floor(sqrt(n)) for n>=0 using binary search. */
static idx_t isqrt_idx(idx_t n)
{
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        if (mid == 0) {
            low = 1;
            continue;
        }
        if (mid > n / mid) {
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }
    return high;  /* floor sqrt of n */
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1 */
    idx_t s = isqrt_idx(n);
    /* round sqrt to nearest integer: floor(sqrt(n)+0.5) */
    if (n > s * s + s) ++s;
    return (seq_t)(2 * s + 1);
}

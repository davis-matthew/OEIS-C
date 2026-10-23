/*
 * A190335  a(n) = n + [n*s/r] + [n*t/r]; r=2, s=sqrt(2), t=1/s.
 * https://oeis.org/A190335
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + ⌊n/√2⌋ + ⌊n/√8⌋ (equivalently, a(n) = n + ⌊n·√2/2⌋ +
 *            ⌊n·√2/4⌋)
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 8219, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* integer square root: floor(sqrt(x)) for non‑negative x */
static seq_t isqrt(seq_t x)
{
    seq_t low = 0;
    seq_t high = x;
    while (low < high) {
        /* mid = ceil((low+high)/2) */
        seq_t mid = low + ((high - low + 1) >> 1);
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    /* compute s = floor(√(2·n²)) = floor(n·√2) */
    seq_t x = nn * nn;
    x = x * (seq_t)2;
    seq_t s = isqrt(x);
    /* floor(n/√2) = floor(n·√2/2) = s/2
       floor(n/√8) = floor(n·√2/4) = s/4   */
    return nn + (s >> 1) + (s >> 2);
}

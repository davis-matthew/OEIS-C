/*
 * A083657  a(n) = Sum_{i=1..n} floor(r*floor(r*i)) where r=sqrt(3).
 * https://oeis.org/A083657
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{i=1..n} floor(sqrt(3) * floor(sqrt(3) * i))
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root: floor(sqrt(x)) */
static seq_t isqrt_seq(seq_t x)
{
    seq_t low = 0;
    seq_t high = x;
    while (low < high) {
        seq_t mid = low + (high - low + 1) / 2;
        seq_t sq = mid * mid;
        if (sq <= x)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* Compute a(n) for n >= 1 */
seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 1; i <= n; ++i) {
        /* t = floor(sqrt(3) * i) = floor(sqrt(3 * i * i)) */
        seq_t t = isqrt_seq((seq_t)((unsigned __int128)3 * (unsigned __int128)i * i));
        /* term = floor(sqrt(3) * t) = floor(sqrt(3 * t * t)) */
        seq_t term = isqrt_seq((seq_t)((unsigned __int128)3 * (unsigned __int128)t * t));
        sum += term;
    }
    return sum;
}

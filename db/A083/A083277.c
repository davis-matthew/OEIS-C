/*
 * A083277  k appears 3k-2 times.
 * https://oeis.org/A083277
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((7 + floor(sqrt(24*n - 23))) / 6)
 * Range:     u8 n <= 92, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root for unsigned __int128.
   Returns floor(sqrt(x)).  The result fits in 64 bits
   because x ≤ 24*2^64 in the usage below. */
static inline uint64_t isqrt_uint128(unsigned __int128 x)
{
    unsigned __int128 low  = 0;
    unsigned __int128 high = ((unsigned __int128)1) << 64;  /* 2^64 */

    while (low <= high) {
        unsigned __int128 mid = (low + high) >> 1;
        if (mid == 0) {
            low = 1;
            continue;
        }
        if (mid <= x / mid)          /* mid*mid ≤ x  without overflow   */
            low = mid + 1;
        else
            high = mid - 1;
    }
    return (uint64_t)high;
}

/* Compute the n‑th term of OEIS sequence A083277.
   n is 1‑based and must be ≥ 1. */
seq_t a(idx_t n)
{
    /* 24*n - 23 fits easily into an unsigned __int128. */
    unsigned __int128 v = ((unsigned __int128)24 * n) - 23u;

    /* floor(sqrt(24*n - 23)) */
    uint64_t s = isqrt_uint128(v);

    /* floor((7 + s) / 6) – the final term. */
    return (seq_t)((s + 7) / 6);
}

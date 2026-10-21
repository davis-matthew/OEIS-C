/*
 * A240025  Characteristic function of quarter squares, cf. A002620.
 * https://oeis.org/A240025
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n is a quarter‐square (exists k with
 *            floor(k^2/4)=n), otherwise 0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root: floor(sqrt(n)) for 0 <= n <= 2^64-1. */
static idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0;
    idx_t high = (idx_t)1 << 32;   /* ceil(sqrt(2^64-1)) < 2^32 */
    while (low < high) {
        idx_t mid = low + ((high - low + 1u) >> 1);
        if (mid <= n / mid)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* Ceiling of sqrt(4*n).  Returns the smallest k with k^2 >= 4*n. */
static idx_t ceil_sqrt4n(idx_t n)
{
    idx_t s = isqrt_u64(n);          /* floor(sqrt(n)) */
    idx_t k = 2 * s;
    unsigned __int128 fourn = (unsigned __int128)n * 4;
    while ((unsigned __int128)k * k < fourn)
        ++k;
    return k;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    idx_t k = ceil_sqrt4n(n);
    unsigned __int128 fourn = (unsigned __int128)n * 4;
    if ((unsigned __int128)k * k <= fourn + 3)
        return (seq_t)1;
    else
        return (seq_t)0;
}

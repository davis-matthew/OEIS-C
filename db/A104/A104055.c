/*
 * A104055  Number of numbers 0 <= i <= n such that i is a square or a cube
 *          (or both).
 * https://oeis.org/A104055
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1 + floor(sqrt(n)) + floor(cuberoot(n)) - floor(n^(1/6))
 * Range:     u8 n <= 103, u16 n <= 103, u32 n <= 103, u64 n <= 103,
 *            u128 n <= 103, big any n
 * Helpers:   icbrt_idx, isixth_root_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/icbrt_idx.h"
#include "../utils/isixth_root_idx.h"

/* Integer floor square root of an idx_t number */
static idx_t isqrt_idx(idx_t n)
{
    idx_t low = 0;
    idx_t high = n;
    while (low <= high)
    {
        idx_t mid = low + ((high - low) >> 1);
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq <= n)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

/* Sequence A104055: number of integers 0 ≤ i ≤ n that are square or cube */
seq_t a(idx_t n)
{
    idx_t sq   = isqrt_idx(n);
    idx_t cb   = icbrt_idx(n);
    idx_t sixth = isixth_root_idx(n);
    seq_t result = (seq_t)1 + (seq_t)sq + (seq_t)cb - (seq_t)sixth;
    return result;
}

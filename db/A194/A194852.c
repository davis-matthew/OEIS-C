/*
 * A194852  Number of primes in the interval [ sqrt(n^3) , sqrt((n+1)^3) ].
 * https://oeis.org/A194852
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   number of primes in the integer interval [⌈√(n³)⌉ , ⌊√((n+1)³)⌋]
 * Range:     u8 n <= 2, u16 n <= 24, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Validated: partial, pass, 87 terms through n = 87 (2026-11-01)
 * Helpers:   ipow_i32, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow_i32.h"
#include "../utils/is_prime.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    /* Compute n³ as seq_t */
    seq_t sn = (seq_t)n;
    sn *= sn;
    sn *= (seq_t)n;

    /* Lower bound: ceil(sqrt(n³)) */
    seq_t low_sqrt = floor_sqrt(sn);
    seq_t low;
    if (low_sqrt * low_sqrt < sn)
        low = low_sqrt + 1;
    else
        low = low_sqrt;

    /* Upper bound: floor(sqrt((n+1)³)) */
    idx_t np1 = n + 1;
    seq_t sn2 = (seq_t)np1;
    sn2 *= sn2;
    sn2 *= (seq_t)np1;
    seq_t high = floor_sqrt(sn2);

    /* Count primes in [low, high] using the provided prime test */
    idx_t low_i = (idx_t)low;
    idx_t high_i = (idx_t)high;

    seq_t count = 0;
    for (idx_t p = low_i; p <= high_i; ++p)
    {
        if (is_prime(p))
            ++count;
    }

    return count;
}

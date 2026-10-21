/*
 * A074279  n appears n^2 times.
 * https://oeis.org/A074279
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   a(n) = smallest m such that m*(m+1)*(2*m+1)/6 >= n (for n ≥ 1)
 * Range:     u8 n <= 9455, u16 n <= 9455, u32 n <= 9455, u64 n <= 9455,
 *            u128 n <= 9455, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the definition “n appears n^2 times” as a partial sum of squares. */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* Undefined for n=0; behaviour below offset is irrelevant */

    unsigned long long low  = 1;
    unsigned long long high = 1;

    /* Find an upper bound where the cumulative sum of squares reaches or exceeds n */
    while ( ((unsigned __int128)high * (high + 1) * (2 * high + 1) / 6) < n )
    {
        high <<= 1;
        /* Guard against overflow of the 64‑bit shift (should not happen for n≤2^64-1) */
        if (high == 0)
        {
            high = (unsigned long long)-1;
            break;
        }
    }

    /* Binary search for the minimal m with sum_{i=1..m} i^2 ≥ n */
    while (low + 1 < high)
    {
        unsigned long long mid = low + (high - low) / 2;
        unsigned __int128 sum = (unsigned __int128)mid * (mid + 1) * (2 * mid + 1) / 6;
        if (sum >= n)
            high = mid;
        else
            low = mid;
    }

    return (seq_t)high;
}

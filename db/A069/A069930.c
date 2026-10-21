/*
 * A069930  Number of integers of the form (n+k)/(n-k) with 1 <= k <= n-1.
 * https://oeis.org/A069930
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = #{ d : d | 2*n and d < n } (number of divisors of 2n that
 *            are less than n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalently, the number of k (1 ≤ k ≤ n-1) for which (n+k)/(n-k) is an integer. */

#include <stdint.h>

/* Helper: Integer square‑root for unsigned 64‑bit numbers.
   Uses binary search with 128‑bit intermediate to avoid overflow. */
static idx_t int_sqrt_u64(idx_t x)
{
    idx_t lo = 0, hi = (idx_t)1 << 32;  /* hi^2 > max idx_t for all idx_t values */
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    /* two_n = 2*n; use 128‑bit to avoid overflow for large n. */
    unsigned __int128 two_n = (unsigned __int128)n * 2;

    /* limit = floor(sqrt(two_n)) */
    idx_t limit = int_sqrt_u64((idx_t)two_n);

    seq_t count = 0;

    for (idx_t d = 1; d <= limit; ++d) {
        if ((two_n % d) != 0)
            continue;

        /* d is a divisor of 2n */
        if (d < n)
            ++count;

        idx_t d2 = (idx_t)(two_n / d);
        if (d2 != d && d2 < n)
            ++count;
    }

    return count;
}

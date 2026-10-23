/*
 * A133196  n+2 repeated n times.
 * https://oeis.org/A133196
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((sqrt(8*n-7)-1)/2)+3
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* offset is 1; for safety return 0 when n == 0 */
    if (n == 0)
        return (seq_t)0;

    idx_t low = 1;
    idx_t high = n;          /* k never exceeds n */

    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        /* use 128‑bit product to avoid overflow: mid*(mid+1)/2 */
        __uint128_t prod = (__uint128_t)mid * (mid + 1) / 2;

        if (prod >= n)
            high = mid;
        else
            low = mid + 1;
    }

    /* low is the smallest k with k(k+1)/2 ≥ n */
    return (seq_t)(low + 2);
}

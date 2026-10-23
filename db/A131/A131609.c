/*
 * A131609  Mirror image of triangle in A131606.
 * https://oeis.org/A131609
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = (Fib(r))^k where r = smallest integer with (r+1)(r+2)/2 >
 *            n, and k = n - r*(r+1)/2 (0‑based index within the row)
 * Range:     u8 n <= 18, u16 n <= 26, u32 n <= 43, u64 n <= 75, u128 n <= 133,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/fib.h"   /* seq_t fib(idx_t n) */

seq_t a(idx_t n)
{
    /* Find row r such that cumulative(r‑1) <= n < cumulative(r) */
    idx_t low  = 0;
    idx_t high = 1;

    /* Grow high until it covers n, but prevent overflow */
    while (1) {
        __int128 cumHigh = (__int128)(high + 1) * (high + 2) / 2;
        if (cumHigh > n)
            break;
        if (high > ((idx_t)-1) >> 1) {   /* doubling would overflow */
            high = (idx_t)-1;
            break;
        }
        high <<= 1;
    }

    /* Binary search for r */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        __int128 cumMid = (__int128)(mid + 1) * (mid + 2) / 2;
        if (cumMid <= n)
            low = mid + 1;
        else
            high = mid;
    }
    idx_t r = low;   /* r is the row where n lies */

    /* Offset within the row */
    idx_t prevCum = (idx_t)((__int128)r * (r + 1) / 2);  /* cumulative up to row r-1 */
    idx_t offset  = n - prevCum;                         /* 0‑based index in row r */

    /* Compute Fibonacci(r) */
    seq_t base = fib(r);        /* Fib(r) – may be 0 for r==0 */
    seq_t result = 1;           /* accumulator */

    /* Fast exponentiation: result = base^offset */
    idx_t exp = offset;
    while (exp > 0) {
        if (exp & 1)
            result = result * base;
        base = base * base;
        exp >>= 1;
    }

    return result;
}

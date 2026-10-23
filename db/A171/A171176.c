/*
 * A171176  Triangle read by rows in which row n lists 3n-1 together with the
 *          first 2n-1 positive integers, in reverse order.
 * https://oeis.org/A171176
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   a(n) is the nth element of the triangle whose r‑th row is [3r‑1,
 *            2r‑1, 2r‑2, …, 2, 1]. Row r has length 2r and the cumulative
 *            count up to row r is r(r+1). For a given n find r = smallest
 *            integer with r(r+1) ≥ n, then let offset = n - (r-1)r - 1. If
 *            offset == 0 output 3r‑1; otherwise output 2r - o...
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Find smallest r such that r*(r+1) >= n.  */
static idx_t
find_row(idx_t n)
{
    if (n <= 1) return 1;
    idx_t low  = 1;
    idx_t high = 1;
    /* Expand high until the condition holds. */
    while ( ((__uint128_t)high * (high + 1)) < n ) {
        high <<= 1;
        if (high > n) { high = n; break; }
    }
    /* Binary search for minimal r. */
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        __uint128_t prod = (__uint128_t)mid * (mid + 1);
        if (prod >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

seq_t
a(idx_t n)
{
    if (n == 0u) return 0;  /* undefined for the sequence; guard. */
    idx_t r      = find_row(n);
    idx_t prev   = (r - 1u) * r;          /* terms before row r */
    idx_t offset = n - prev - 1u;         /* 0‑based offset in row r */

    seq_t R = (seq_t)r;
    if (offset == 0u)
        return (seq_t)(3 * R - (seq_t)1);
    else
        return (seq_t)(2 * R - (seq_t)offset);
}

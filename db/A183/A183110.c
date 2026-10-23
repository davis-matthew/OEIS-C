/*
 * A183110  Period-length of the ultimate periodic behavior of the orbit of a
 *          list [1,1,1,...,1] of n 1's under the mapping defined in the
 *          comments.
 * https://oeis.org/A183110
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 250, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = 1 if n is a triangular number; otherwise return the smallest
   k such that k(k+1)/2 >= n. */
#include <stdint.h>
#include <limits.h>

seq_t a(idx_t n)
{
    /* Find the smallest k with k(k+1)/2 >= n. */
    idx_t low  = 0;
    idx_t high = 1;

    /* Exponentially increase the upper bound until it is large enough. */
    while (1) {
        seq_t tri = ((seq_t)high * ((seq_t)high + 1)) / 2;
        if (tri >= n || high == (idx_t)-1) break;
        if (high > (UINT64_MAX >> 1)) { high = (idx_t)-1; break; }
        high <<= 1;
    }

    /* Binary search in [low, high]. */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        seq_t tri = ((seq_t)mid * ((seq_t)mid + 1)) / 2;
        if (tri >= n)
            high = mid;
        else
            low = mid + 1;
    }

    idx_t k   = low;
    seq_t tri = ((seq_t)k * ((seq_t)k + 1)) / 2;
    if (tri == n)
        return (seq_t)1;   /* n is a triangular number */
    else
        return (seq_t)k;   /* otherwise period length = k */
}

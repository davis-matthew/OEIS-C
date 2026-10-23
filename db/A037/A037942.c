/*
 * A037942  Numbers of the form x^2 + y^2 with x >= 0, y >= 0, gcd(x,y)=1,
 *          with multiplicity.
 * https://oeis.org/A037942
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* x^2 + y^2 with x >= 0, y >= 0, gcd(x,y)=1, counted with multiplicity
 * (unordered pairs with y <= x).  The sequence is sorted ascending,
 * a(1) = 1.  The function returns the nth term for n >= 1. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"

static inline idx_t count_leq(seq_t M)
{
    idx_t cnt = 0;
    for (idx_t x = 1; (seq_t)x * (seq_t)x <= M; ++x) {
        seq_t x_sq = (seq_t)x * (seq_t)x;
        for (idx_t y = 0; y <= x; ++y) {
            seq_t sum = x_sq + (seq_t)y * (seq_t)y;
            if (sum > M) break;               /* sums increase with y */
            if (gcd((seq_t)x, (seq_t)y) == (seq_t)1)
                ++cnt;
        }
    }
    return cnt;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* undefined for n=0 */

    seq_t low  = 0;
    seq_t high = 1;

    /* Grow high until we have at least n numbers <= high. */
    while (count_leq(high) < n) {
        high <<= 1;                         /* double */
        if (high == 0) break;               /* overflow guard */
    }

    /* Binary search for the minimal M with count_leq(M) >= n. */
    while (low < high) {
        seq_t mid = low + ((high - low) >> 1);
        if (count_leq(mid) >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

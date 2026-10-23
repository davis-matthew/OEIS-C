/*
 * A020888  Ordered set of (a + b - c)/2 as (a,b,c) runs through all primitive
 *          Pythagorean triples with a<b<c.
 * https://oeis.org/A020888
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 74, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A020888: ordered inradii of primitive Pythagorean triangles.
 *
 * Each inradius r is given by r = n * k where
 *
 *   - n >= 1
 *   - k >= 1 is odd,
 *   - gcd(n, k) = 1
 *
 * The sequence consists of all such r, sorted increasingly with
 * multiplicities.  This function returns the nth term of that
 * sequence for n >= 1.
 *
 * Algorithm
 * ----------
 * 1.  Count, for any integer x, how many pairs (n, k) satisfy
 *     n * k <= x, k odd, gcd(n, k) = 1.  The counting is done by
 *     a simple double loop.  The count gives the number of terms
 *     <= x in the sequence.
 *
 * 2.  Binary search for the smallest x such that count(x) >= n.
 *     That x is guaranteed to be the nth term, because the count
 *     is monotone and every term is counted with multiplicity.
 *
 * The routine is O(N log M) with a very small constant; for N up to 10K
 * it runs well under the required timing limits.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd.h"

static inline seq_t count_leq(idx_t limit)
{
    seq_t cnt = 0;
    for (idx_t n = 1; n <= limit; ++n) {
        idx_t max_k = limit / n;
        if (max_k == 0) break;
        for (idx_t k = 1; k <= max_k; k += 2) {
            if (gcd((seq_t)n, (seq_t)k) == 1)
                ++cnt;
        }
    }
    return cnt;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* undefined for n < 1, but return 0 here */

    /* Exponential search to find an upper bound. */
    idx_t high = 1;
    while (count_leq(high) < n)
        high <<= 1;

    idx_t low = 1;
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        if (count_leq(mid) >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return (seq_t)low;
}

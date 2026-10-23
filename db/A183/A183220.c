/*
 * A183220  Complement of the octagonal numbers.
 * https://oeis.org/A183220
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + ⌊½ + √(n/3)⌋ This is equivalent to a(n) = n + k,
 *            where k is the smallest integer satisfying 3·k·(k+1) ≥ n
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>   /* for bool, not used but required by the build */

seq_t a(idx_t n)
{
    /* Input n is 1‑based.  No special case for n==0 is needed. */

    /* Find the smallest integer k such that 3*k*(k+1) >= n.  We
       use a binary search.  The search space is bounded by an
       exponential “doubling” phase that stops when the condition
       is already satisfied. */
    seq_t high = 1;
    /* avoid overflow:   high <= n / (3*(high+1))  */
    while (high <= (seq_t)n / ( 3 * (high + 1) )) {
        high <<= 1;
    }

    seq_t low = 0;
    while (low < high) {
        seq_t mid = (low + high) >> 1;
        seq_t prod = 3 * mid * (mid + 1);
        if (prod >= (seq_t)n) {
            high = mid;
        } else {
            low = mid + 1;
        }
    }
    /* low is the required k */
    return (seq_t)n + low;
}

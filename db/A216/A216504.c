/*
 * A216504  Number of values of k for which n can be written in the form a^2 +
 *          k*b^2, a >= 0, b >= 0, k > 0. a(n) = 0 if there are infinitely
 *          many such k.
 * https://oeis.org/A216504
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = count of distinct k>0 for which n = a^2 + k*b^2, a,b ≥0.
 *            If n is a perfect square, then a(n)=0 because infinitely many k
 *            satisfy n = a^2 + k*0
 * Range:     u8 n <= 9080, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation iterates over all a from 0 to ⌊√n⌋,
   computes rem = n-a², and for each divisor (b²) of rem
   records the corresponding k = rem/(b²).  A small boolean array
   is used to avoid counting the same k twice.  Complexity is
   roughly O(√n·√n)=O(n).  ε */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

static inline idx_t int_sqrt(idx_t n)
{
    /* Integer square root via simple binary search using __uint128_t
       to avoid overflow on multiplication.  n is 64‑bit, so the
       largest intermediate value is < (2⁶⁴)² which fits in 128 bits. */
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq == n) return mid;
        if (sq < n) low = mid + 1;
        else        high = mid - 1;
    }
    return high;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;               /* safety for n=0 */

    idx_t sq = int_sqrt(n);
    if (sq * sq == n) return (seq_t)0;         /* perfect square → infinitely many k */

    /* Allocate a small boolean array for bookkeeping of seen k's.
       The maximum potential k is n (when b=1). */
    bool *seen = (bool*)calloc((size_t)n + 1, sizeof(bool));
    if (!seen) return (seq_t)0;                /* allocation failure → 0 as a safe fallback */

    seq_t count = 0;

    for (idx_t a_val = 0; a_val <= sq; ++a_val) {
        idx_t rem = n - a_val * a_val;
        if (rem == 0) continue;                 /* would correspond to b=0, already handled */

        idx_t max_b = int_sqrt(rem);
        for (idx_t b = 1; b <= max_b; ++b) {
            idx_t b2 = b * b;
            if (rem % b2 == 0) {
                idx_t k = rem / b2;
                if (k > 0 && !seen[k]) {
                    seen[k] = true;
                    ++count;
                }
            }
        }
    }

    free(seen);
    return count;
}

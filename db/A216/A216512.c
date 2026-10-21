/*
 * A216512  Number of nonnegative integer solutions to the equation a^2 +
 *          7*b^2 = n.
 * https://oeis.org/A216512
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{(a,b) ∈ ℕ₀² : a² + 7·b² = n}
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements a straightforward O(√n) search. */

#include <stdint.h>
#include <stdbool.h>

/* Integer square root for unsigned 64‑bit values.
 * Returns ⌊√x⌋.  Works for all values that fit in idx_t. */
static inline idx_t _int_sqrt(idx_t x)
{
    idx_t low  = 0;
    idx_t high = x;
    idx_t res  = 0;

    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        idx_t sq  = mid * mid;
        if (sq <= x) {
            res = mid;
            low = mid + 1;
        } else {
            /* Avoid underflow: mid > 0 ensures high can be decreased. */
            high = mid - 1;
        }
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Offset starts at 1; for n == 0 return 0 (undefined). */
    if (n == 0) return 0;

    seq_t count = 0;
    idx_t a_limit = _int_sqrt(n);          /* maximal possible a */

    for (idx_t a_val = 0; a_val <= a_limit; ++a_val) {
        idx_t a_sq = a_val * a_val;
        if (a_sq > n) break;

        idx_t rem = n - a_sq;
        if (rem % 7 != 0) continue;

        idx_t b_sq = rem / 7;
        idx_t b_val = _int_sqrt(b_sq);
        if (b_val * b_val == b_sq) {
            ++count;
        }
    }
    return count;
}

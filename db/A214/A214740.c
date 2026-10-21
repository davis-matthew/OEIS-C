/*
 * A214740  Least m>0 such that n-m divides 2^n+m.
 * https://oeis.org/A214740
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n – max{d | 1 ≤ d < n and d ∣ (2ⁿ + n)}
 * Range:     u8 n <= 261, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   pow2_mod_u32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow2_mod_u32.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;                /* undefined for n < 2, return 0 */

    /* search for the largest d < n that divides 2^n + n */
    for (idx_t d = n - 1; d >= 1; --d) {
        /* 2^n mod d */
        uint32_t pow_mod = pow2_mod_u32(n, (uint32_t)d);
        /* (2^n + n) mod d */
        uint32_t rem = (pow_mod + (uint32_t)(n % d)) % (uint32_t)d;
        if (rem == 0)
            return (seq_t)(n - d);      /* m = n - d */
    }
    /* unreachable, but keep compiler happy */
    return (seq_t)0;
}

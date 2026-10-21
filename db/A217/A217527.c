/*
 * A217527  a(n) = 2^(n-2)*(n-2)^2+2^(n-1).
 * https://oeis.org/A217527
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n-2)*(n-2)^2 + 2^(n-1) (for n ≥ 2)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 24, u64 n <= 54, u128 n <= 116,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* If n is below the offset (2) the value is undefined for the
       sequence; return 0 for safety. */
    if (n < 2) {
        return 0;
    }

    /* Compute (n-2) and 2^(n-2).  All intermediate results fit in seq_t
       (the type is at least as wide as idx_t). */
    seq_t m     = n - 2;              // (n-2)
    seq_t pow_m = pow2(n - 2);        // 2^(n-2)
    seq_t win = pow_m * m * m;        // (n-2)^2 * 2^(n-2)

    /* Add the remaining term 2^(n-1) */
    return win + pow2(n - 1);
}

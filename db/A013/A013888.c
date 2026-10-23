/*
 * A013888  a(n) = 18^(5*n + 3).
 * https://oeis.org/A013888
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 18^(5*n + 3)
 * Range:     u8 none, u16 n <= 0, u32 n <= 0, u64 n <= 2, u128 n <= 5,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n) {
    /* Exponent is 5*n + 3.  Use unsigned literals to avoid overflow. */
    idx_t exp = 5ULL * n + 3ULL;
    return pow_uint(18ULL, exp);
}

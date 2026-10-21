/*
 * A055351  Number of labeled mobiles (circular rooted trees) with n nodes and
 *          4 leaves.
 * https://oeis.org/A055351
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * (n-1)^2 * (n-2)^2 * (n-3) * (n-4) / 1152
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 10, u64 n <= 17, u128 n <= 30,
 *            big any n
 * Helpers:   gcd_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd_uint64.h"

/* Euclidean gcd for uint64_t */

seq_t a(idx_t n) {
    if (n < 5) return 0;  /* Undefined below the offset */

    seq_t result = (seq_t)1;
    uint64_t remaining_div = 1152;  /* 2^7 * 3^2 */

    /* Multiply all factors of n! */
    for (idx_t i = 2; i <= n; ++i) {
        uint64_t f = (uint64_t)i;
        uint64_t g = gcd_uint64(f, remaining_div);
        result = result * (seq_t)(f / g);
        remaining_div /= g;
    }

    /* Extra factors (n-1)^2, (n-2)^2, (n-3), (n-4) */
    for (int k = 0; k < 2; ++k) {
        uint64_t f = (uint64_t)n - 1;
        uint64_t g = gcd_uint64(f, remaining_div);
        result = result * (seq_t)(f / g);
        remaining_div /= g;
    }
    for (int k = 0; k < 2; ++k) {
        uint64_t f = (uint64_t)n - 2;
        uint64_t g = gcd_uint64(f, remaining_div);
        result = result * (seq_t)(f / g);
        remaining_div /= g;
    }
    {
        uint64_t f = (uint64_t)n - 3;
        uint64_t g = gcd_uint64(f, remaining_div);
        result = result * (seq_t)(f / g);
        remaining_div /= g;
    }
    {
        uint64_t f = (uint64_t)n - 4;
        uint64_t g = gcd_uint64(f, remaining_div);
        result = result * (seq_t)(f / g);
        remaining_div /= g;
    }

    /* Remaining divisor should be 1; divide if needed */
    if (remaining_div > 1) {
        result /= (seq_t)remaining_div;
    }

    return result;
}

/*
 * A039332  Numbers whose base-8 representation has the same nonzero number of
 *          0's and 4's.
 * https://oeis.org/A039332
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   has_equal_nonzero_zeros_and_fours
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers whose base-8 representation has the same non-zero
 * number of 0's and 4's.  a(1) = 32, a(2) = 68, ...
 * The function returns the n-th term (n ≥ 1) by brute force search.
 * No dynamic allocation is required, so it works with all supplied
 * seq_t types (C integer types or arbitrary-precision C++ classes). */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/has_equal_nonzero_zeros_and_fours.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;     /* offset is 1 – guard against bad input   */
    idx_t count = 0;
    idx_t candidate = 1;
    while (count < n) {
        if (has_equal_nonzero_zeros_and_fours(candidate)) {
            count++;
            if (count == n) {
                return (seq_t)candidate;
            }
        }
        candidate++;
    }
    /* Should never reach here for valid n */
    return (seq_t)0;
}

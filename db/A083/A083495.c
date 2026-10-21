/*
 * A083495  Multiples of 7 in which there is no common digit in successive
 *          terms.
 * https://oeis.org/A083495
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 28, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   common_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: successive multiples of 7 that share no common digit with the previous term */
#include <stdbool.h>
#include "../utils/common_digits.h"

seq_t a(idx_t n)
{
    seq_t current = 7;          /* a(0) */
    if (n == 0) return current;

    for (idx_t i = 1; i <= n; ++i) {
        seq_t candidate = current + 7;      /* next multiple of 7 */
        while (common_digits(current, candidate))
            candidate += 7;                 /* skip until digits disjoint */
        current = candidate;
    }
    return current;
}

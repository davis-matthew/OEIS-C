/*
 * A037368  Numbers whose base-7 and base-9 expansions have no digits in
 *          common.
 * https://oeis.org/A037368
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 101, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   has_disjoint_base_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: find the nth positive integer whose base-7 and base-9 expansions have no digits in common */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/has_disjoint_base_digits.h"

/* External linkage as required */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* not used but safe */

    idx_t found = 0;
    uint64_t m = 1;

    while (1) {
        if (has_disjoint_base_digits(m)) {
            ++found;
            if (found == n) {
                return (seq_t)m;
            }
        }
        ++m;
    }
}

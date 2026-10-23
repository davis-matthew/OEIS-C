/*
 * A047053  a(n) = 4^n * n!.
 * https://oeis.org/A047053
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^n * n!
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 13, u128 n <= 24,
 *            big any n
 * Helpers:   seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/seq_mul.h"

seq_t a(idx_t n) {
    /* a(0) = 1 by definition */
    seq_t result = 1;
    for (idx_t i = 1; i <= n; ++i) {
        /* multiply by 4*i each step: 4^n * n! = ∏(4*i) */
        result = seq_mul(result, (seq_t)(4 * i));
    }
    return result;
}

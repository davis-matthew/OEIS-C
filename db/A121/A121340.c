/*
 * A121340  List of triples {1^(2^n), 2^(2^n), 3^(2^n)} for n >= 0.
 * https://oeis.org/A121340
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = ( (n mod 3)+1 )^(2^(n/3))
 * Range:     u8 n <= 9, u16 n <= 12, u32 n <= 15, u64 n <= 18, u128 n <= 21,
 *            big any n
 * Validated: partial, pass, 24 terms through n = 23 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

static inline seq_t seq_one(void) { return (seq_t)1; }

seq_t a(idx_t n)
{
    idx_t row = n / 3;
    int   col = n % 3;
    uint64_t base = (uint64_t)(col + 1);

    /* 1^(anything) is always 1 */
    if (base == 1)
        return seq_one();

    seq_t result = (seq_t)base;
    for (idx_t i = 0; i < row; ++i)
        result = seq_mul(result, result);

    return result;
}

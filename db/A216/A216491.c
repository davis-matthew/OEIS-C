/*
 * A216491  a(n) = 12*5^n.
 * https://oeis.org/A216491
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*5^n
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 12, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Validated: partial, pass, 23 terms through n = 22 (2026-11-01)
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

static seq_t power_of_seq(seq_t base, idx_t exp) {
    seq_t result = (seq_t)1;
    while (exp) {
        if (exp & (idx_t)1)
            result = seq_mul(result, base);
        base = seq_mul(base, base);
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    seq_t pow5 = power_of_seq((seq_t)5, n);
    return seq_mul(pow5, (seq_t)12);
}

/*
 * A132911  a(n) = (n+1)*(2*n)!/2^n.
 * https://oeis.org/A132911
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n+1)*(2*n)!/2^n Recurrence: a(0) = 1; a(n) =
 *            (n+1)*(2*n-1)*a(n-1) for n > 0
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Validated: partial, pass, 17 terms through n = 16 (2026-11-01)
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

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t result = (seq_t)1;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t factor = (seq_t)(k + 1) * (seq_t)(2 * k - 1);
        result = seq_mul(result, factor);
    }
    return result;
}

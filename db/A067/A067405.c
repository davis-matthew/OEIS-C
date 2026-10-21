/*
 * A067405  Fifth column of triangle A067402.
 * https://oeis.org/A067405
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*25^(n-1) for n>=1; a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the known closed–form for OEIS sequence A067405 */

#include <stdint.h>

/* Helper: binary exponentiation for seq_t */
static seq_t pow_seq_t(seq_t base, idx_t exp) {
    seq_t result = (seq_t)1;
    while (exp > 0) {
        if (exp & 1U) result *= base;
        base *= base;
        exp >>= 1U;
    }
    return result;
}

/* Sequence function */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    return (seq_t)9 * pow_seq_t((seq_t)25, n - 1);
}

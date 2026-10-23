/*
 * A166023  a(n) = 6*a(n-2) for n > 2; a(1) = 1, a(2) = 5.
 * https://oeis.org/A166023
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-2) (n>2); a(1)=1, a(2)=5
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 25, u64 n <= 49, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute 6^exp using binary exponentiation.  exp is non‑negative. */
static seq_t pow6(idx_t exp) {
    seq_t result = (seq_t)1;
    seq_t base   = (seq_t)6;
    while (exp) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)5;
    if (n & 1) {             /* n is odd (>=3) */
        idx_t e = (n - 1) / 2;
        return pow6(e);
    } else {                 /* n is even (>=4) */
        idx_t e = (n / 2) - 1;
        return pow6(e) * (seq_t)5;
    }
}

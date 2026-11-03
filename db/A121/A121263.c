/*
 * A121263  Descending dungeons: see Comments lines for definition.
 * https://oeis.org/A121263
 *
 * Offset:    10    (a(10) is the first term)
 * Keywords:  nonn, nice, base
 * Formula:   a(n) = 10_(11_(12_(..._(n-1_n)...))) // nested base
 *            interpretation
 * Range:     u8 n <= 23, u16 n <= 30, u32 n <= 40, u64 n <= 55, u128 n <= 79,
 *            big any n
 * Validated: standard, pass, 51 terms through n = 105 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Interpret the decimal representation of num in base 'base'.
   num is an idx_t (uint64_t), base and the result are seq_t. */
static seq_t decimal_in_base(idx_t num, seq_t base) {
    int digits[20];
    int len = 0;
    if (num == 0) {
        digits[len++] = 0;
    } else {
        while (num > 0) {
            digits[len++] = num % 10;
            num /= 10;
        }
    }
    seq_t value = 0;
    for (int i = len - 1; i >= 0; --i) {
        value = value * base + digits[i];
    }
    return value;
}

/* Compute sequence a(n) for n >= 10.
   For n < 10 the result is defined as 0 (unused). */
seq_t a(idx_t n) {
    if (n < 10u) return 0;
    if (n == 10u) return (seq_t)10;
    /* Start with the innermost pair (n-1)_n */
    seq_t val = decimal_in_base(n - 1u, (seq_t)n);
    /* Process all outer levels down to 10_... */
    for (idx_t k = n - 2u; k >= 10u; --k) {
        val = decimal_in_base(k, val);
    }
    return val;
}

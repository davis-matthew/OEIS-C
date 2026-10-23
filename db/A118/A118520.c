/*
 * A118520  Define sequence S_m by: initial term = m, reverse digits and add 3
 *          to get next term. Entry shows S_5. This reaches a cycle of length
 *          6 in 2 steps.
 * https://oeis.org/A118520
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=5; a(n)=rev(a(n‑1))+3 for n>1, where rev reverses decimal digits. */
#include <stdint.h>

/* Reverse the decimal digits of a value held in seq_t. */
static inline seq_t reverse_digits(seq_t x) {
    seq_t rev = 0;
    while (x > 0) {
        rev = rev * 10 + x % 10;
        x = x / 10;
    }
    return rev;
}

seq_t a(idx_t n) {
    seq_t val = (seq_t)5;
    for (idx_t i = 1; i < n; ++i) {
        val = reverse_digits(val) + (seq_t)3;
    }
    return val;
}

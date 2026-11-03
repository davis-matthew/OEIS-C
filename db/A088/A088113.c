/*
 * A088113  a(n) = digit reversal of (11^n) divided by 11.
 * https://oeis.org/A088113
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = reverse_decimal(11^n) / 11
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Validated: partial, pass, 18 terms through n = 18 (2026-11-01)
 * Helpers:   ipow, reverse_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A088113: a(n) = digit reversal of (11^n) divided by 11. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/ipow.h"
#include "../utils/reverse_digits.h"

seq_t a(idx_t n)
{
    /* 11^n in seq_t (big_pow handles large exponents) */
    seq_t pow_val = ipow((seq_t)11, (seq_t)n);

    /* Digit reversal of the decimal representation */
    seq_t rev = reverse_digits(pow_val);

    /* Divide by 11 to obtain a(n) */
    return rev / (seq_t)11;
}

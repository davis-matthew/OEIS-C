/*
 * A088113  a(n) = digit reversal of (11^n) divided by 11.
 * https://oeis.org/A088113
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = reverse_decimal(11^n) / 11
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   big_pow, reverse_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A088113: a(n) = digit reversal of (11^n) divided by 11. */

#include "../utils/big_pow.h"
#include "../utils/reverse_decimal.h"

seq_t a(idx_t n)
{
    /* 11^n in seq_t (big_pow handles large exponents) */
    seq_t pow_val = big_pow((seq_t)11, (seq_t)n);

    /* Digit reversal of the decimal representation */
    seq_t rev = reverse_decimal(pow_val);

    /* Divide by 11 to obtain a(n) */
    return rev / (seq_t)11;
}

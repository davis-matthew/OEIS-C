/*
 * A095515  a(n) = 61 written in base 15 - n.
 * https://oeis.org/A095515
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = representation of 61 in base (15 - n), with digits
 *            interpreted as decimal digits
 * Range:     u8 n <= 10, u16 n <= 12, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* For n > 14 the base would be <= 0, undefined → return 0. */
    if (n > 14)
        return 0;

    idx_t base = 15 - n;          /* 1 … 15 */
    const seq_t VALUE = 61;       /* the constant to be converted */

    /* Unary base (base == 1): output 111…1 (61 ones). */
    if (base == 1) {
        seq_t out = 0;
        for (idx_t i = 0; i < VALUE; ++i)
            out = out * 10 + 1;
        return out;
    }

    /* Convert VALUE to base 'base' */
    seq_t b = (seq_t)base;      /* cast base to seq_t for arithmetic |
                                 * values are small (≤ 15). */
    uint8_t digits[70];          /* 61 digits max for unary case, 70 is safe. */
    size_t len = 0;
    seq_t temp = VALUE;
    while (temp > 0) {
        seq_t rem_seq = temp % b;
        uint8_t rem = (uint8_t)rem_seq;   /* digit value (0 … base-1) */
        digits[len++] = rem;
        temp = temp / b;
    }

    /* Build the decimal number by concatenating the digits. */
    seq_t out = 0;
    for (size_t i = len; i > 0; --i)
        out = out * 10 + digits[i - 1];

    return out;
}

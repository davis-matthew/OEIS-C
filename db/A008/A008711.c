/*
 * A008711  14 in base 14-n.
 * https://oeis.org/A008711
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  fini, nonn, base, full
 * Formula:   a(n) = decimal value of the representation of 14 in base (14-n)
 * Range:     u8 n <= 11, u16 n <= 12, u32 n <= 12, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for OEIS sequence A008711. */
seq_t a(idx_t n)
{
    /* For n>14 the base would be <=0, undefined. Return 0. */
    if (n > 14) {
        return 0;
    }

    /* Base of the representation. */
    uint64_t base = 14 - n;

    /* Special case: base 1 (unary). 14 in base 1 is 14 ones. */
    if (base <= 1) {
        seq_t result = 0;
        /* Build 14 consecutive '1' digits. */
        for (uint64_t i = 0; i < 14; ++i) {
            result = result * 10 + 1;
        }
        return result;
    }

    /* General case: base ≥ 2.  Convert 14 to base 'base', then interpret
       the digit string as a decimal number. */
    seq_t x = 14;         /* current value to convert. */
    seq_t result = 0;     /* running decimal value. */
    seq_t pow10  = 1;     /* 10^position multiplier. */

    while (x > 0) {
        seq_t digit = x % (seq_t)base;   /* least significant digit. */
        result += digit * pow10;         /* place it in decimal form. */
        pow10 *= 10;                     /* move to next decimal place. */
        x /= (seq_t)base;                /* shift to next digit. */
    }

    return result;
}

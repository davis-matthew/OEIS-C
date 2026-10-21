/*
 * A095473  a(n) = 40 written in base 13 - n.
 * https://oeis.org/A095473
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = representation of 40 in base (13 - n)
 * Range:     u8 n <= 9, u16 n <= 10, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base = 13 - n. For n > 12 the base would be <= 0,
       which is not defined for this sequence.  Return 0 in that case. */
    if (n > 12) {
        return 0;
    }

    idx_t b = 13 - n;            /* base, 1 .. 13 */
    seq_t result = 0;

    if (b == 1) {                      /* unary: 40 ones */
        for (idx_t i = 0; i < 40; ++i) {
            result = result * (seq_t)10 + (seq_t)1;
        }
        return result;
    }

    /* For bases 2..13, compute the digits of 40 in that base. */
    seq_t val = (seq_t)40;
    if (val == 0) {
        return 0;
    }

    /* Collect remainders (digits) in reverse order. */
    uint8_t digits[64];   /* plenty of space */
    idx_t digits_cnt = 0;

    seq_t base = (seq_t)b;

    while (val != 0) {
        uint8_t rem = (uint8_t)(val % base);
        digits[digits_cnt++] = rem;
        val = val / base;
    }

    /* Reverse digits to get most significant first and build decimal number. */
    for (idx_t i = 0; i < digits_cnt; ++i) {
        result = result * (seq_t)10 + (seq_t)digits[digits_cnt - 1 - i];
    }

    return result;
}

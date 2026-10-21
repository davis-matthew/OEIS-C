/*
 * A095589  a(n) = 98 written in base 10 - n.
 * https://oeis.org/A095589
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = representation of 98 in base (10 - n) written as a
 *            decimal integer (for base 1, return 98 ones). For n≥0 the
 *            function yields the sequence A095589
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base of representation */
    idx_t base = 10 - n;

    /* Base 1: unary expansion – 98 ones */
    if (base == 1) {
        seq_t val = 0;
        for (idx_t i = 0; i < 98; ++i)
            val = val * 10 + 1;
        return val;
    }

    /* Bases 2..9: compute digits of 98 in that base */
    /* Highest number of digits for 98 in base 2 is 7, but reserve 64 for safety */
    uint8_t digits[64];
    idx_t len = 0;
    seq_t temp = 98;               /* the number to convert */

    /* Convert 98 to the desired base */
    while (temp > 0) {
        seq_t d = temp % base;     /* digit in the new base */
        temp /= base;
        digits[len++] = (uint8_t)d;
    }
    if (len == 0)                 /* if n >= 10 (invalid), return 0 */
        return 0;

    /* Assemble the decimal integer from the digits (most significant first) */
    seq_t val = 0;
    for (idx_t i = len; i > 0; --i)
        val = val * 10 + digits[i - 1];

    return val;
}

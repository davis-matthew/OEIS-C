/*
 * A095573  a(n) = 90 written in base 12 - n.
 * https://oeis.org/A095573
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = the decimal integer formed by writing 90 in base (12‑n),
 *            i.e. the base‑(12-n) representation of 90, with each digit
 *            interpreted as a decimal digit. For n = 11 (base 1) the
 *            representation is the unary string of 90 digits “1”. For n > 11
 *            the base is ≤ 0, so the function returns 0
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* base for the conversion */
    int base = 12 - (int)n;

    /* base <= 0: undefined – return 0 */
    if (base <= 0) return 0;

    /* base == 1: unary representation of 90 (90 ones) */
    if (base == 1) {
        seq_t r = 0;
        for (int i = 0; i < 90; ++i)
            r = r * 10 + 1;
        return r;
    }

    /* base >= 2: convert 90 to the desired base and build decimal value */
    seq_t temp = 90;                /* number to convert (in decimal) */
    /* store digits in reverse order */
    int digits[32];                 /* 90 fits into at most 7 digits in base 2 */
    int dlen = 0;
    while (temp > 0) {
        int d = (int)(temp % base); /* d < base <= 10 for n >= 0 */
        digits[dlen++] = d;
        temp = temp / base;
    }

    /* assemble decimal integer from most significant digit */
    seq_t result = 0;
    for (int i = dlen - 1; i >= 0; --i)
        result = result * 10 + digits[i];

    return result;
}

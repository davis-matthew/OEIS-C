/*
 * A095507  a(n) = 57 written in base 14 - n.
 * https://oeis.org/A095507
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = decimal number whose digits are the representation of 57
 *            in base (14 - n)
 * Range:     u8 n <= 9, u16 n <= 11, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is defined only for n = 0 … 13 (bases 14 … 1). */
    if (n > 13)
        return 0;

    idx_t base = 14 - n;          /* base of the representation */
    if (base <= 1)                /* only base == 1 for n == 13 */
    {
        seq_t result = 0;
        for (idx_t i = 0; i < 57; ++i)
            result = result * 10 + 1;   /* 57 unary ones */
        return result;
    }

    /* Base >= 2 : convert decimal 57 into that base. */
    idx_t val = 57;
    int digits[10];               /* enough for base 2: 6 digits */
    int len = 0;
    while (val > 0)
    {
        digits[len++] = (int)(val % base);
        val /= base;
    }

    /* Build the decimal integer that has exactly those digits. */
    seq_t result = 0;
    for (int i = len - 1; i >= 0; --i)
        result = result * 10 + digits[i];

    return result;
}

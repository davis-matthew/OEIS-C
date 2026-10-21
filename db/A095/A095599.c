/*
 * A095599  a(n) = 104 written in base 14 - n.
 * https://oeis.org/A095599
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = decimal interpretation of 104 written in base (14 - n)
 * Range:     u8 n <= 8, u16 n <= 11, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Guard against underflow: base positive only when n <= 13 */
    if (n > 14) return 0u;
    idx_t base = 14 - n;
    if (base == 0) return 0u;          /* base 0 undefined */
    if (base == 1) {
        /* Unary representation: 104 ones ⇒ value with 104 decimal 1's */
        seq_t val = 0;
        for (idx_t i = 0; i < 104; ++i) {
            val = val * 10 + 1;
        }
        return val;
    }

    /* Convert 104 (decimal) to base 'base' */
    unsigned int digits[7];             /* 104 in binary needs at most 7 digits */
    idx_t len = 0;
    unsigned int x = 104;
    while (x > 0) {
        digits[len++] = x % (unsigned int)base;
        x /= base;
    }

    /* Interpret digits as a decimal string: most significant first */
    seq_t val = 0;
    for (idx_t i = len; i-- > 0;) {
        val = val * 10 + digits[i];
    }
    return val;
}

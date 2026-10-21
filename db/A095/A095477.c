/*
 * A095477  a(n) = 42 written in base 14 - n.
 * https://oeis.org/A095477
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = the decimal integer whose decimal digits are the *
 *            base-(14‑n) representation of 42. For example: * n = 0 → base 14
 *            : 42 = 30₁₄ → a(0) = 30 * n = 1 → base 13 : 42 = 33₁₃ → a(1) =
 *            33 * n = 8 → base 6 : 42 = 110₆ → a(8) = 110 * n = 12 → base 2 :
 *            42 = 101010₂ → a(12) = 101010 * n = 13 → base...
 * Range:     u8 n <= 10, u16 n <= 11, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Determine the base in which 42 will be expressed. */
    int base = (int)(14 - n);
    if (base <= 0)          /* base 0 or negative – undefined */
        return 0;

    /* Base‑1 (unary) representation: 42 ones. */
    if (base == 1) {
        seq_t val = 0;
        for (idx_t i = 0; i < 42; ++i) {
            val = val * 10 + 1;
        }
        return val;
    }

    /* Convert 42 to the required base and build the decimal number
     * formed by the base digits.  All digits are < 10, so they
     * can be concatenated as decimal digits. */
    seq_t temp = 42;
    int digits[64];   /* 42 in base 2 requires 6 digits, 64 is safe */
    int len = 0;

    while (temp > 0) {
        int d = (int)(temp % base);
        digits[len++] = d;
        temp /= base;
    }
    if (len == 0)      /* never happens for 42 > 0 */
        return 0;

    seq_t result = 0;
    for (int i = len - 1; i >= 0; --i) {
        result = result * 10 + digits[i];
    }
    return result;
}

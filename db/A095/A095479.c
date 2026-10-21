/*
 * A095479  a(n) = 43 written in base 10 - n.
 * https://oeis.org/A095479
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = decimal number obtained by writing 43 in base (10 - n)
 * Range:     u8 n <= 6, u16 n <= 7, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base of representation: b = 10 - n   (n = 0 .. 9) */
    idx_t b = 10 - n;
    if (b == 0 || b > 10)   /* undefined for n > 9 or n < 0 (unsigned) */
        return 0;

    /* Special case: base 1 (unary) – 43 ones */
    if (b == 1) {
        seq_t res = 0;
        for (idx_t i = 0; i < 43; ++i)
            res = res * 10 + 1;
        return res;
    }

    /* For bases 2..10: convert 43 to base b, then build decimal value */
    idx_t temp = 43;
    int digits[64];          /* enough for all bases here (max 43 digits for base 1) */
    int len = 0;
    while (temp > 0) {
        digits[len++] = temp % b;
        temp /= b;
    }

    seq_t res = 0;
    for (int i = len - 1; i >= 0; --i)
        res = res * 10 + (seq_t)digits[i];

    return res;
}

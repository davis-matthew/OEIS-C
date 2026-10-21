/*
 * A095535  a(n) = 71 written in base 11 - n.
 * https://oeis.org/A095535
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = 71 written in base 11 - n
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n > 10) return 0;                 /* base would be ≤ 0, undefined */
    seq_t base = (seq_t)(11 - n);
    if (base == 1) {                      /* unary representation */
        seq_t res = 0;
        for (int i = 0; i < 71; ++i)       /* repeat digit '1' 71 times */
            res = res * 10 + 1;
        return res;
    }
    /* base >= 2 */
    seq_t temp = 71;
    unsigned char digits[71];
    int pos = 0;
    while (temp > 0) {                     /* extract base–digits (LSB first) */
        digits[pos++] = (unsigned char)(temp % base);
        temp /= base;
    }
    seq_t res = 0;
    for (int i = pos - 1; i >= 0; --i)      /* form decimal number */
        res = res * 10 + digits[i];
    return res;
}

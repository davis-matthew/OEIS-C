/*
 * A095555  a(n) = 81 written in base 13 - n.
 * https://oeis.org/A095555
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = the decimal integer obtained by writing 81 in base (13 -
 *            n) and interpreting the digits as decimal
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n >= (idx_t)13)
        return 0;
    int base = 13 - (int)n;
    if (base == 1) {
        seq_t res = 0;
        for (idx_t i = 0; i < 81; ++i)
            res = res * 10 + 1;
        return res;
    }
    /* base >= 2 */
    int digits[20];
    int len = 0;
    int v = 81;
    while (v > 0) {
        digits[len++] = v % base;
        v /= base;
    }
    seq_t res = 0;
    for (int i = len - 1; i >= 0; --i)
        res = res * 10 + digits[i];
    return res;
}

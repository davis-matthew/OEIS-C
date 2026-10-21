/*
 * A095447  a(n) = 27 written in base 13 - n.
 * https://oeis.org/A095447
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = decimal integer formed by writing 27 in base (13‑n) and
 *            interpreting the resulting digit string as a decimal number. For
 *            0 ≤ n ≤ 12 this yields the sequence terms: 21, 23, 25, 27, 30,
 *            33, 36, 43, 102, 123, 1000, 11011, 111111111111111111111111111
 *            For n > 12 the expression is not defined; t...
 * Range:     u8 n <= 9, u16 n <= 11, u32 n <= 11, u64 n <= 11, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n > 12) return 0;
    /* base = 13 - n (n fits in 0..12, so int is safe) */
    int base = 13 - (int)n;
    if (base >= 2) {
        int val = 27;
        int dig[32];
        int len = 0;
        while (val > 0) {
            dig[len++] = val % base;
            val /= base;
        }
        seq_t res = 0;
        for (int i = len - 1; i >= 0; --i) {
            res = res * 10 + dig[i];
        }
        return res;
    } else {               /* base == 1 : unary representation of 27 */
        seq_t res = 0;
        for (int i = 0; i < 27; ++i) {
            res = res * 10 + 1;
        }
        return res;
    }
}

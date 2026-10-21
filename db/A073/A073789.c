/*
 * A073789  Numbers in base -8.
 * https://oeis.org/A073789
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = decimal digits of n expressed in base -8
 * Range:     u8 n <= 71, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;
    }

    /* Convert n to base -8 using signed arithmetic (valid for n up to 10000). */
    long long x = (long long)n;
    int digits[64];          /* enough for any 64‑bit number in base -8 */
    int pos = 0;

    while (x != 0) {
        long long r = x % -8;   /* remainder may be negative */
        x /= -8;
        if (r < 0) {
            r += 8;            /* make remainder positive */
            x += 1;            /* adjust quotient */
        }
        digits[pos++] = (int)r;
    }

    /* Build the decimal number described by the digit array (most significant first). */
    seq_t result = 0;
    for (int i = pos - 1; i >= 0; --i) {
        result = result * 10 + (seq_t)digits[i];
    }

    return result;
}

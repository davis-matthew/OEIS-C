/*
 * A043001  Base-3 palindromes that start with 1.
 * https://oeis.org/A043001
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth palindrome in base 3 that starts with 1 (in decimal)
 * Range:     u8 n <= 18, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t remaining = n;
    idx_t length = 1;
    while (1) {
        idx_t half = (length + 1) / 2;
        idx_t countL;
        if (length == 1) {
            countL = 1;
        } else {
            /* 3^(half - 1) */
            idx_t p = 1;
            for (idx_t i = 0; i < half - 1; ++i) p *= 3;
            countL = p;
        }
        if (remaining <= countL) {
            /* Build the desired palindrome */
            idx_t offset = remaining - 1;               /* 0‑based index within this length */
            unsigned char halfDigits[64];
            halfDigits[0] = 1;                           /* leading digit */
            for (idx_t i = half - 1; i > 0; --i) {       /* fill second half‑digits */
                halfDigits[i] = offset % 3;
                offset /= 3;
            }
            seq_t value = 0;
            for (idx_t pos = 0; pos < length; ++pos) {
                seq_t d;
                if (pos < half) {
                    d = halfDigits[pos];
                } else {
                    d = halfDigits[length - 1 - pos];
                }
                value = value * 3 + d;
            }
            return value;
        }
        remaining -= countL;
        ++length;
    }
}

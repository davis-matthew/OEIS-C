/*
 * A095518  a(n) = 63 written in base n.
 * https://oeis.org/A095518
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = representation of 63 in base n, interpreted as a decimal
 *            integer (each base‑n digit is written in decimal, concatenated)
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 none, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle base 1 (unary): 63 has 63 ones. */
    if (n == 1) {
        seq_t res = 0;
        for (idx_t i = 0; i < 63; ++i) {
            res = res * 10 + 1;
        }
        return res;
    }

    /* Convert 63 to base n. 63 is small, so at most 7 digits are needed
       (base 2 gives 6 bits).  Use a static buffer of sufficient size. */
    /* digits[0] = least significant digit, digits[digits-1] = most significant */
    idx_t digits[128];
    idx_t len = 0;
    seq_t num = 63;

    while (num > 0) {
        idx_t digit = (idx_t)(num % (seq_t)n);
        digits[len++] = digit;
        num = num / (seq_t)n;
    }

    /* Build the decimal integer by concatenating the digits. */
    seq_t res = 0;
    for (idx_t i = len; i > 0; --i) {
        res = res * 10 + (seq_t)digits[i - 1];
    }
    return res;
}

/*
 * A202022  Characteristic functions of repdigit numbers in decimal
 *          representation.
 * https://oeis.org/A202022
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 1 if n is a repdigit in decimal, else 0
 * Range:     u8 n <= 4673, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)              /* 0 itself is a repdigit */
        return (seq_t)1;
    seq_t num = (seq_t)n;    /* work with seq_t for big integer support */
    seq_t last = num % 10;   /* first (least significant) digit */
    num /= 10;
    while (num > 0) {
        if (num % 10 != last)    /* digits differ => not a repdigit */
            return (seq_t)0;
        num /= 10;
    }
    return (seq_t)1;        /* all digits were equal */
}

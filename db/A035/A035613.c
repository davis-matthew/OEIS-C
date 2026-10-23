/*
 * A035613  7 in base n.
 * https://oeis.org/A035613
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = decimal value of the representation of decimal 7 in base
 *            n
 * Range:     u8 none, u16 none, u32 n <= 93, u64 n <= 93, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 1111111, a(2) = 111, a(3) = 21, a(4) = 13, a(5) = 12, a(6) = 11, a(7) = 10, a(n≥8) = 7 */

seq_t a(idx_t n) {
    /* Base 1 is unary: seven '1's */
    if (n == 1) {
        return 1111111;
    }

    seq_t rem = 7;
    seq_t result = 0;
    seq_t pow10 = 1;
    seq_t base = (seq_t)n;

    /* Convert 7 to base `n` and interpret each digit as a decimal digit */
    while (rem > 0) {
        seq_t digit = rem % base;       /* least significant digit in base `n` */
        result += digit * pow10;        /* build the decimal number */
        pow10 *= 10;                    /* advance decimal place */
        rem /= base;
    }

    return result;
}

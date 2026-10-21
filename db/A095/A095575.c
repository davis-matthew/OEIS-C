/*
 * A095575  a(n) = 91 written in base 15 - n.
 * https://oeis.org/A095575
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = decimal integer obtained by interpreting the base-(15-n)
 *            representation of 91 as a decimal number
 * Range:     u8 n <= 9, u16 n <= 12, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const seq_t value = 91;                /* the number to convert            */
    const idx_t  base  = 15 - n;           /* base used for the conversion     */

    /* For bases <= 0 the representation is undefined */
    if (base <= 0)
        return 0;

    /* Prepare a buffer for the base‑style digits.  The maximum
       number of digits for 91 in base 1 is 91.                                      */
    uint8_t digits[91];
    idx_t   len = 0;

    if (base == 1) {
        /* Unary: 91 ones */
        for (idx_t i = 0; i < 91; ++i) {
            digits[i] = 1;
        }
        len = 91;
    } else {
        /* Standard base conversion for base >= 2. */
        seq_t tmp = value;
        while (tmp > 0) {
            seq_t d = tmp % base;          /* remainder is a digit          */
            digits[len++] = (uint8_t)d;
            tmp = tmp / base;
        }
    }

    /* Interpret the digit sequence as a decimal integer. */
    seq_t result = 0;
    for (idx_t i = len; i > 0; --i) {
        result = result * (seq_t)10 + (seq_t)digits[i - 1];
    }
    return result;
}

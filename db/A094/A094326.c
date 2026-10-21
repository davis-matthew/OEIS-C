/*
 * A094326  a(n) = 8 written in base n.
 * https://oeis.org/A094326
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = integer that is the representation of decimal 8 in base n
 *            (base 1 is the unary system, giving eight 1’s)
 * Range:     u8 none, u16 none, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n == 0 the offset is not defined – return 0. */
    if (n == 0) {
        return (seq_t)0;
    }

    /* Base–1 is the unary system.  8 is written as eight 1’s. */
    if (n == (idx_t)1) {
        /* 11111111 as a decimal literal. */
        return (seq_t)11111111U;
    }

    /* For n >= 2 compute 8 in base n.  The number 8 is tiny, so the
       conversion is trivial and fast. */
    uint64_t tmp = 8ULL;           /* value to convert (decimal 8)   */
    unsigned char digits[10];      /* digits in base n (reversed)    */
    unsigned len = 0;              /* number of digits               */

    /* Extract digits in base n. */
    while (tmp > 0) {
        digits[len++] = (unsigned char)(tmp % (uint64_t)n);
        tmp /= (uint64_t)n;
    }

    /* Build the decimal number from the most significant digit. */
    seq_t result = 0;
    while (len > 0) {
        --len;
        result = result * 10 + (seq_t)digits[len];
    }
    return result;
}

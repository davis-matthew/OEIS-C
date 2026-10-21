/*
 * A095635  a(n) = 122 written in base 11 - n.
 * https://oeis.org/A095635
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = decimal representation of the representation of 122 in
 *            base (11 - n). For 0 ≤ n ≤ 10 this yields the sequence:
 *            101,122,145,172,233,322,442,1322,11112,1111010,111…111 (122
 *            ones)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is defined only for 0 ≤ n ≤ 10. */
    if (n > 10 || n < 0) return 0;

    int base = 11 - (int)n;          /* base to use when representing 122 */
    if (base <= 0) return 0;

    int value = 122;                  /* the decimal value to encode */

    /* Store digits of the representation (least significant first). */
    int digits[200];                  /* enough for base = 1 case (122 digits) */
    int len = 0;

    if (base == 1) {                 /* unary representation: all 1's */
        for (int i = 0; i < value; ++i) {
            digits[len++] = 1;
        }
    } else {
        while (value > 0) {
            digits[len++] = value % base;
            value /= base;
        }
    }

    /* Build the decimal integer from the digit array. */
    seq_t result = 0;
    for (int i = len - 1; i >= 0; --i) {
        result = result * 10 + (seq_t)digits[i];
    }

    return result;
}

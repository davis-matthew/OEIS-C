/*
 * A095475  a(n) = 41 written in base 13 - n.
 * https://oeis.org/A095475
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = digits of 41 in base (13-n) interpreted as a decimal
 *            integer
 * Range:     u8 n <= 9, u16 n <= 10, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 41 is a constant that fits in any seq_t */
    const seq_t num = 41;

    /* Base for the conversion: 13 - n */
    idx_t base = (idx_t)13 - n;

    /* Undefined for base <= 0 */
    if (base <= 0)
        return 0;

    /* Special handling for base 1 (unary representation) */
    if (base == 1) {
        seq_t result = 0;
        /* 41 is the constant value; produce that many '1's */
        for (idx_t i = 0; i < num; ++i)
            result = result * 10 + 1;
        return result;
    }

    /* Convert num to base `base` and store the digits (least significant first) */
    seq_t digits[64];       /* maximum needed for num=41 in any base >=2 */
    idx_t digit_cnt = 0;
    seq_t value = num;
    while (value > 0) {
        digits[digit_cnt++] = value % base;
        value /= base;
    }

    /* Now interpret the digits as a decimal number */
    seq_t result = 0;
    for (idx_t i = digit_cnt; i > 0; --i)
        result = result * 10 + digits[i - 1];

    return result;
}

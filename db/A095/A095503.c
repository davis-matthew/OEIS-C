/*
 * A095503  a(n) = 55 written in base 13 - n.
 * https://oeis.org/A095503
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = the decimal value of the string obtained by writing 55 in
 *            base (13 − n)
 * Range:     u8 n <= 8, u16 n <= 10, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* base = 13 - n */
    int64_t base = 13 - (int64_t)n;
    /* Special cases */
    if (base == 1) {
        /* Unary representation: 55 ones */
        seq_t r = 0;
        for (int i = 0; i < 55; ++i) {
            r = r * 10 + 1;
        }
        return r;
    }
    if (base == 0) {
        /* Base 0 not defined – return 0 */
        return 0;
    }
    if (base == -1) {
        /* Base -1 representation is problematic – return 0 for safety */
        return 0;
    }

    /* 55 must be converted to the given base (positive or negative). */
    int64_t val = 55;
    /* Store up to 8 digits – 55 in any base will need at most 3 digits for |base|>1. */
    int digits[16];
    int len = 0;
    int64_t abs_base = base > 0 ? base : -base;

    while (val != 0) {
        int64_t r = val % abs_base;
        if (r < 0) r += abs_base;      /* ensure r in [0, abs_base-1] */
        digits[len++] = (int)r;
        val = (val - r) / base;        /* exact division since val - r is divisible by base */
    }
    if (len == 0) {
        /* val was 0 – then representation is single digit 0 */
        digits[len++] = 0;
    }

    /* Build the decimal number from the most significant digit to the least. */
    seq_t result = 0;
    for (int i = len - 1; i >= 0; --i) {
        int d = digits[i];
        if (d < 10) {
            result = result * 10 + d;
        } else {
            /* Digit larger than 9 – write its decimal digits sequentially. */
            char buf[20];
            int pos = 0;
            int tmp = d;
            while (tmp > 0) {
                buf[pos++] = '0' + (tmp % 10);
                tmp /= 10;
            }
            for (int j = pos - 1; j >= 0; --j) {
                result = result * 10 + (buf[j] - '0');
            }
        }
    }
    return result;
}

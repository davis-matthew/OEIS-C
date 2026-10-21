/*
 * A095583  a(n) = 95 written in base 11 - n.
 * https://oeis.org/A095583
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = digits of 95 written in base (11 - n), interpreted as a decimal integer. */

seq_t a(idx_t n) {
    /* 11 - n must stay >= 1.  For n > 10 the sequence is undefined. */
    if (n > 10) return 0;

    /* Base for representation */
    unsigned base = 11u - (unsigned)n;

    seq_t value = 0;

    /* Unary case (base 1) – 95 copies of digit 1 */
    if (base == 1u) {
        /* Construct the decimal number consisting of 95 ones */
        for (unsigned i = 0; i < 95; ++i) {
            value = value * 10 + 1;
        }
        return value;
    }

    /* Convert 95 (decimal) to base `base` and store digits in reverse order. */
    unsigned x = 95;
    int digits[32];
    int cnt = 0;
    while (x > 0) {
        digits[cnt++] = (int)(x % base);
        x /= base;
    }
    /* Build the decimal number by concatenating the digits. */
    for (int i = cnt - 1; i >= 0; --i) {
        value = value * 10 + digits[i];
    }
    return value;
}

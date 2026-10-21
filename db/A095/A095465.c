/*
 * A095465  a(n) = 36 written in base 12 - n.
 * https://oeis.org/A095465
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = representation of decimal 36 in base (12 - n)
 * Range:     u8 n <= 8, u16 n <= 9, u32 n <= 10, u64 n <= 10, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Sequence is only defined for 0 ≤ n ≤ 11 (bases 12 down to 1). */
    if (n > 11u)
        return 0;

    unsigned base = 12u - (unsigned)n;
    seq_t result = 0;

    /* Unary representation (base 1) – 36 ones. */
    if (base == 1u) {
        for (unsigned i = 0; i < 36u; ++i)
            result = result * (seq_t)10 + (seq_t)1;
        return result;
    }

    /* Convert 36 to the given base and build the decimal integer that
       has the same digit string. */
    int value = 36;
    int digits[10];
    int len = 0;
    while (value > 0) {
        digits[len++] = value % (int)base;
        value /= (int)base;
    }

    for (int i = len - 1; i >= 0; --i)
        result = result * (seq_t)10 + (seq_t)digits[i];

    return result;
}

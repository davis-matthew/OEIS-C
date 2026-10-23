/*
 * A008714  17 in base 17-n.
 * https://oeis.org/A008714
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  fini, nonn, base, full
 * Formula:   a(n) = decimal value of the representation of decimal 17 in base
 *            (17 - n) (for 0 ≤ n ≤ 16). For n > 16 the sequence is undefined;
 *            this implementation returns 0
 * Range:     u8 n <= 14, u16 n <= 15, u32 n <= 15, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* base  = 17 - n  (must be ≥ 1) */
    if (n > 16) {
        /* out of defined range */
        return 0;
    }
    seq_t base = (seq_t)17 - (seq_t)n;

    /* base == 1: unary representation of 17 → seventeen '1's */
    if (base == 1) {
        seq_t result = 0;
        for (int i = 0; i < 17; ++i) {
            result = result * 10 + 1;
        }
        return result;
    }

    /* 2 ≤ base ≤ 17: convert 17 to the given base. */
    seq_t value = 17;
    /* digits collected least significant first */
    int digits[64];          /* enough to hold all digits */
    int dcnt = 0;
    while (value > 0) {
        seq_t digit = value % base;
        value /= base;
        digits[dcnt++] = (int)digit;
    }

    /* build the decimal number from most significant to least */
    seq_t result = 0;
    for (int i = dcnt - 1; i >= 0; --i) {
        result = result * 10 + digits[i];
    }
    return result;
}

/*
 * A084342  Digit reversal of n concatenated with n mod n.
 * https://oeis.org/A084342
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = (rev(n) * 10^digits(n)) mod n where rev(n) reverses the
 *            decimal digits of n
 * Range:     u8 n <= 10, u16 n <= 165, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* compute rev(n) and the number of digits of n */
    seq_t rev = 0;
    idx_t digits = 0;
    idx_t tmp = n;
    while (tmp > 0) {
        rev = rev * 10 + (tmp % 10);
        tmp /= 10;
        ++digits;
    }

    /* compute 10^digits(n) */
    seq_t pow10 = 1;
    for (idx_t i = 0; i < digits; ++i) pow10 *= 10;

    /* a(n) = rev(n) * 10^digits(n) mod n */
    seq_t value = rev * pow10;
    return value % (seq_t)n;
}

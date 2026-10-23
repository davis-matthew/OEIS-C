/*
 * A116667  Greatest digit not used in n (or 10 if n is pandigital).
 * https://oeis.org/A116667
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = greatest digit not used in the decimal representation of
 *            n, or 10 if all digits 0–9 appear
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    unsigned used = 0;

    /* Handle zero separately: its only digit is 0. */
    if (n == 0) {
        used = 1u << 0;
    } else {
        idx_t x = n;
        while (x > 0) {
            unsigned digit = (unsigned)(x % 10u);
            used |= 1u << digit;
            x /= 10u;
        }
    }

    /* Find the greatest unused digit. */
    for (int d = 9; d >= 0; --d) {
        if (!(used & (1u << d))) {
            return (seq_t)d;
        }
    }

    /* All digits used: return 10. */
    return (seq_t)10;
}

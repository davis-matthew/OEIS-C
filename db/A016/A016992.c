/*
 * A016992  a(n) = (7*n)^12.
 * https://oeis.org/A016992
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7*n)^12
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 0, u64 n <= 5, u128 n <= 232,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* compute x = 7*n as seq_t */
    seq_t x = (seq_t)7 * n;
    /* result = x^12 via repeated multiplication */
    seq_t result = 1;
    for (unsigned i = 0; i < 12; ++i) {
        result *= x;
    }
    return result;
}

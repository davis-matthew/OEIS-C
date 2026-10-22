/*
 * A007662  Quadruple factorial numbers n!!!!: a(n) = n*a(n-4).
 * https://oeis.org/A007662
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * a(n-4), a(0) = 1, a(1) = 1, a(2) = 2, a(3) = 3
 * Range:     u8 n <= 11, u16 n <= 18, u32 n <= 32, u64 n <= 55, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 1;
    for (idx_t i = n; i > 0; ) {
        result *= (seq_t)i;
        if (i < 4) break;
        i -= 4;
    }
    return result;
}

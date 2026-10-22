/*
 * A002301  a(n) = n! / 3.
 * https://oeis.org/A002301
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*a(n-1), a(3) = 2 (i.e. a(n) = 2 * 4*5*...*n = n!/3)
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 2;
    for (idx_t i = 4; i <= n; i++) {
        result *= (seq_t)i;
    }
    return result;
}

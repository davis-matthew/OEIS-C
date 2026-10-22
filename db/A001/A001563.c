/*
 * A001563  a(n) = n*n! = (n+1)! - n!.
 * https://oeis.org/A001563
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = n*n!
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 1;
    for (seq_t i = 2; i <= (seq_t)n; i++) {
        result *= i;
    }
    result *= (seq_t)n;
    return result;
}

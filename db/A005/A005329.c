/*
 * A005329  a(n) = Product_{i=1..n} (2^i - 1). Also called 2-factorial
 *          numbers.
 * https://oeis.org/A005329
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = Product_{i=1..n} (2^i - 1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 1;
    for (idx_t i = 1; i <= n; i++) {
        result *= (((seq_t)1 << i) - 1);
    }
    return result;
}

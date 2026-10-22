/*
 * A007661  Triple factorial numbers a(n) = n!!!, defined by a(n) = n*a(n-3),
 *          a(0) = a(1) = 1, a(2) = 2. Sometimes written n!3.
 * https://oeis.org/A007661
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = Product_{i=0..floor((n-1)/3)} (n - 3*i)
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 26, u64 n <= 44, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 1;
    for (idx_t i = 0; i < (n + 2) / 3; i++) {
        result *= ((seq_t)n - 3 * (seq_t)i);
    }
    return result;
}

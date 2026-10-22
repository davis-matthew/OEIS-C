/*
 * A000723  Invertible Boolean functions of n variables.
 * https://oeis.org/A000723
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2^n - 1)! / n! = product from k = n+1 to 2^n - 1 of k
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 1;
    idx_t limit = (1ULL << n) - 1;
    for (idx_t i = n + 1; i <= limit; i++) {
        result *= (seq_t)i;
    }
    return result;
}

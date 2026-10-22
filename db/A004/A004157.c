/*
 * A004157  Sum of digits of n-th triangular number.
 * https://oeis.org/A004157
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A007953(A000217(n)) — sum of digits of n-th triangular
 *            number
 * Range:     u8 n <= 22, u16 n <= 526, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    // Compute the n-th triangular number: T(n) = n*(n+1)/2
    seq_t tri = (seq_t)n * ((seq_t)n + 1) / 2;
    
    // Sum the digits of tri
    seq_t digit_sum = 0;
    while (tri > 0) {
        digit_sum += tri % 10;
        tri /= 10;
    }
    return digit_sum;
}

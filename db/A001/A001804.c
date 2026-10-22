/*
 * A001804  a(n) = n! * C(n,2).
 * https://oeis.org/A001804
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n! * C(n,2) = n! * n * (n-1) / 2
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 18, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t fact = 1;
    for (idx_t i = 2; i <= n; i++) {
        fact *= (seq_t)i;
    }
    
    seq_t binom = (seq_t)n * ((seq_t)n - 1) / 2;
    return fact * binom;
}

/*
 * A001048  a(n) = n! + (n-1)!.
 * https://oeis.org/A001048
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*(n-1)!
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 1;
    for (idx_t i = 2; i < n; i++) {
        result *= (seq_t)i;
    }
    return result * ((seq_t)n + 1);
}

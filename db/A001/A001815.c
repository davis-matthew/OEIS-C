/*
 * A001815  a(n) = binomial(n,2) * 2^(n-1).
 * https://oeis.org/A001815
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n * (n-1) * 2^(n-2)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 24, u64 n <= 54, u128 n <= 116,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n < 2) return 0;
    seq_t result = (seq_t)n * ((seq_t)n - 1);
    result = result << (n - 2);
    return result;
}

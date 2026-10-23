/*
 * A100764  a(1) = 1, a(2) = 2, a(3) = 3, a(n) = least number not the sum of
 *          three or fewer previous terms.
 * https://oeis.org/A100764
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n for n <= 3; a(n) = 6*n - 17 for n > 3
 * Range:     u8 n <= 45, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Keep n in seq_t to avoid intermediate overflow */
    if (n <= (idx_t)3) {
        return (seq_t)n;
    } else {
        return ((seq_t)n * (seq_t)6) - (seq_t)17;
    }
}

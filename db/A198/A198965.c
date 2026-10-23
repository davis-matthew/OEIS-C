/*
 * A198965  a(n) = 7*9^n - 1.
 * https://oeis.org/A198965
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*9^n - 1
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 1;
    for (idx_t i = 0; i < n; ++i) {
        result *= 9;          /* multiply by 9 n times to get 9^n */
    }
    result *= 7;              /* result = 7*9^n */
    result -= 1;              /* result = 7*9^n - 1 */
    return result;
}

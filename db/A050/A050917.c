/*
 * A050917  a(n) = n*6^n + 1.
 * https://oeis.org/A050917
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*6^n + 1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* binary exponentiation to compute 6^n in seq_t */
    seq_t result = 1;
    seq_t base   = (seq_t)6;
    idx_t exp    = n;
    while (exp) {
        if (exp & 1) {
            result *= base;
        }
        base *= base;
        exp >>= 1;
    }
    /* compute n * 6^n + 1 */
    return (seq_t)n * result + 1;
}

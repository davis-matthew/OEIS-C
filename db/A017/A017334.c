/*
 * A017334  a(n) = (10*n + 5)^6.
 * https://oeis.org/A017334
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10*n + 5)^6
 * Range:     u8 none, u16 n <= 0, u32 n <= 3, u64 n <= 162, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t base = ((seq_t)n * 10) + 5;   /* compute 10*n + 5 */
    seq_t result = base * base;         /* base^2   */
    result *= base;                    /* base^3   */
    result *= base;                    /* base^4   */
    result *= base;                    /* base^5   */
    result *= base;                    /* base^6   */
    return result;
}

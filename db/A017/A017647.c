/*
 * A017647  a(n) = (12*n+10)^7.
 * https://oeis.org/A017647
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (12*n+10)^7
 * Range:     u8 none, u16 none, u32 n <= 1, u64 n <= 46, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* compute base = 12*n + 10 */
    seq_t base = (seq_t)12 * (seq_t)n + (seq_t)10;

    /* compute base^7 by repeated multiplication */
    seq_t result = base;
    result *= base;  /* base^2 */
    result *= base;  /* base^3 */
    result *= base;  /* base^4 */
    result *= base;  /* base^5 */
    result *= base;  /* base^6 */
    result *= base;  /* base^7 */

    return result;
}

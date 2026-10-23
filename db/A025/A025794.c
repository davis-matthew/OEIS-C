/*
 * A025794  Expansion of 1/((1-x)*(1-x^11)*(1-x^12)).
 * https://oeis.org/A025794
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^2 + 24*n + 12*(2+(n mod 11))*(11-(n mod 11)) - 11*(n
 *            mod 12)*(12-(n mod 12)))/264 (Number of partitions of n into
 *            parts 1, 11, and 12)
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t x = (seq_t)n;          /* n as seq_t */
    idx_t r11 = n % 11;          /* n mod 11 */
    idx_t r12 = n % 12;          /* n mod 12 */

    /* Compute numerator of the formula */
    seq_t numerator = x * x;                /* n^2 */
    numerator += 24 * x;                    /* + 24*n */
    numerator += (seq_t)12 * ((seq_t)(2 + r11)) * ((seq_t)(11 - r11));   /* + 12*(2+(n mod 11))*(11-(n mod 11)) */
    numerator -= (seq_t)11 * ((seq_t)r12) * ((seq_t)(12 - r12));         /* - 11*(n mod 12)*(12-(n mod 12)) */

    /* Division by 264 (constant) */
    return numerator / (seq_t)264;
}

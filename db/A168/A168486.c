/*
 * A168486  Numbers that are congruent to {2, 5} mod 11.
 * https://oeis.org/A168486
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (22*n - 5*(-1)^n - 19)/4
 * Range:     u8 n <= 12, u16 n <= 3038, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* compute 22*n in the target type to avoid overflow */
    seq_t val = (seq_t)n * (seq_t)22;

    /* add or subtract 5 depending on parity of n */
    if (n & (idx_t)1) {
        val += (seq_t)5;      /* n odd: subtract -5 */
    } else {
        val -= (seq_t)5;      /* n even: subtract 5 */
    }

    /* complete the numerator */
    val -= (seq_t)19;

    /* integer division by 4 (exact for all n≥1) */
    return val / (seq_t)4;
}

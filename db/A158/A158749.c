/*
 * A158749  a(n) = n*9^n.
 * https://oeis.org/A158749
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*9^n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t power = (seq_t)1;
    /* Compute 9^n using repeated multiplication */
    for (idx_t i = 0; i < n; ++i) {
        power *= (seq_t)9;
    }
    return (seq_t)n * power;
}

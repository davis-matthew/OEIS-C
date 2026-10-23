/*
 * A122656  a(n) = n*floor(n/2)^2.
 * https://oeis.org/A122656
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*floor(n/2)^2
 * Range:     u8 n <= 10, u16 n <= 63, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to seq_t */
    seq_t m = (seq_t)n;
    /* floor(n/2) as integer division */
    seq_t k = (seq_t)(n / 2);
    /* Compute n * floor(n/2)^2 */
    return m * k * k;
}

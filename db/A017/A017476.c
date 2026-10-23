/*
 * A017476  a(n) = (11*n + 7)^4.
 * https://oeis.org/A017476
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11*n + 7)^4
 * Range:     u8 none, u16 n <= 0, u32 n <= 22, u64 n <= 5957, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert n to seq_t before any arithmetic to avoid overflow */
    seq_t base = (seq_t)11 * n + (seq_t)7;

    /* Compute base squared and then square again for the fourth power */
    seq_t base2 = base * base;
    return base2 * base2;
}

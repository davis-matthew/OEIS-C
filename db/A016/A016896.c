/*
 * A016896  a(n) = (5*n + 3)^12.
 * https://oeis.org/A016896
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5*n + 3)^12
 * Range:     u8 none, u16 none, u32 n <= 0, u64 n <= 7, u128 n <= 324,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute base = 5*n + 3 */
    seq_t base = ((seq_t)5 * (seq_t)n) + ((seq_t)3);
    /* Compute base^12 by repeated multiplication */
    seq_t result = (seq_t)1;
    for (int i = 0; i < 12; ++i)
        result *= base;
    return result;
}

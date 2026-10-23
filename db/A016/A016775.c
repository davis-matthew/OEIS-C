/*
 * A016775  a(n) = (3*n)^11.
 * https://oeis.org/A016775
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n)^11
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 2, u64 n <= 18, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute m = 3*n in seq_t type */
    seq_t m = (seq_t)3 * (seq_t)n;
    /* raise m to the 11th power */
    seq_t result = 1;
    for (idx_t i = 0; i < 11; ++i)
        result *= m;
    return result;
}

/*
 * A017551  a(n) = (12*n + 2)^7.
 * https://oeis.org/A017551
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (12*n + 2)^7
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 46, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the base (12*n + 2) in the target type */
    seq_t base = ((seq_t)n) * 12 + 2;

    /* Raise base to the 7th power */
    seq_t result = base;            /* result = base^1 */
    for (idx_t i = 1; i < 7; ++i)   /* multiply 6 more times: base^7 */
        result *= base;

    return result;
}

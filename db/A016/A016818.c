/*
 * A016818  a(n) = (4n+1)^6.
 * https://oeis.org/A016818
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4n+1)^6
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to seq_t before any arithmetic */
    seq_t base = ((seq_t)n * ((seq_t)4)) + ((seq_t)1);
    seq_t result = ((seq_t)1);

    /* Multiply base by itself six times to compute (4n+1)^6 */
    for (idx_t i = (idx_t)0; i < (idx_t)6; ++i)
    {
        result *= base;
    }

    return result;
}

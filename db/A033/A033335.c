/*
 * A033335  a(n) = floor(15/n).
 * https://oeis.org/A033335
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor(15/n)
 * Range:     u8 n <= 92, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Ensure undefined behaviour is avoided for n == 0,
       even though the sequence is only defined for n >= 1. */
    if (n == 0)
        return 0;
    return ((seq_t)15) / n;
}

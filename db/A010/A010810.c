/*
 * A010810  22nd powers: a(n) = n^22.
 * https://oeis.org/A010810
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = n^22
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 7, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute n to the 22nd power by repeated multiplication. */
    seq_t res  = (seq_t)1;
    seq_t base = (seq_t)n;
    for (idx_t i = 0; i < 22; ++i)
    {
        res *= base;
    }
    return res;
}

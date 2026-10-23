/*
 * A033421  a(n) = floor(1000/n).
 * https://oeis.org/A033421
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor(1000/n)
 * Range:     u8 none, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;              /* Undefined for n=0; safe default */
    return ((seq_t)1000) / ((seq_t)n);
}

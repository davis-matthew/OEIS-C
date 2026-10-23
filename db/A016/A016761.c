/*
 * A016761  a(n) = (2*n+1)^9.
 * https://oeis.org/A016761
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n+1)^9
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 5, u64 n <= 68, u128 n <= 9555,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to the sequence type first to avoid undefined
       behaviour when n is large.  Then compute (2*n+1)^9 by repeated
       multiplication.  The operations +, *, etc. are defined for seq_t
       by the build environment. */
    seq_t base = ((seq_t)n * 2) + 1;
    seq_t result = base;
    int i;
    for (i = 1; i < 9; ++i)
        result *= base;
    return result;
}

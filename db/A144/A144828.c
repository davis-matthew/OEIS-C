/*
 * A144828  Partial products of successive terms of A017113; a(0)=1.
 * https://oeis.org/A144828
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n * (2n)! / n!
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent recurrence: a(0)=1; a(n)=a(n-1)*4*(2n-1) */

seq_t a(idx_t n)
{
    seq_t result = 1;
    for (idx_t i = 1; i <= n; ++i)
    {
        /* factor = 4 * (2*i - 1) */
        seq_t factor = ((seq_t)4) * ((seq_t)(2*i - 1));
        result *= factor;
    }
    return result;
}

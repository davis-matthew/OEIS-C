/*
 * A144756  Partial products of successive terms of A017101; a(0)=1 .
 * https://oeis.org/A144756
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = product_{k=0}^{n-1} (8*k+3) * Recurrence: a(0)=1, a(n) =
 *            (8*n-5)*a(n-1) for n>=1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = (seq_t)1;
    for (idx_t i = 0; i < n; ++i)
    {
        seq_t factor = (seq_t)(8*i + 3);
        result *= factor;
    }
    return result;
}

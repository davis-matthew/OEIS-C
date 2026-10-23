/*
 * A157660  a(n) = 8000*n - 40.
 * https://oeis.org/A157660
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8000*n - 40
 * Range:     u8 none, u16 n <= 8, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the linear function directly.  The cast to seq_t ensures
       the calculation remains within the target type, relying on the
       usual arithmetic conversions.  This expression evaluates in O(1)
       and is valid for all n >= 1, which is the required domain of the
       sequence.  */
    return ((seq_t)8000 * (seq_t)n) - (seq_t)40;
}

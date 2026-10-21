/*
 * A221574  Number of 0..n arrays of length 3 with each element differing from
 *          at least one neighbor by something other than 1.
 * https://oeis.org/A221574
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^3 - n^2 + 3*n - 1
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(1) is the first term; behaviour for n==0 is undefined,
       but we return 0 for safety. */
    if (n == 0)
        return (seq_t)0;

    seq_t nn   = (seq_t)n;
    seq_t term = nn * nn * nn - nn * nn + (seq_t)3 * nn - (seq_t)1;
    return term;
}

/*
 * A157923  a(n) = 49*n^2 - n.
 * https://oeis.org/A157923
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 49*n^2 - n (offset 1)
 * Range:     u8 n <= 2, u16 n <= 36, u32 n <= 9362, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /*  The sequence is defined for n >= 1. For n == 0 we return 0
        which is harmless because the offset is 1 and such a value
        will never be used in a b-file check. */
    if (n == 0) return 0;

    seq_t nn = (seq_t)n;          /* convert index to sequence type */
    return nn * (49 * nn - 1);    /* a(n) = n * (49*n - 1) */
}

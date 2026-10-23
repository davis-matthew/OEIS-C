/*
 * A132126  Number of nonassociative subloops of order 8n of the Cayley
 *          octonions (up to isomorphism).
 * https://oeis.org/A132126
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 9928, u16 n <= 9928, u32 n <= 9928, u64 n <= 9928,
 *            u128 n <= 9928, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A132126: Number of nonassociative subloops of order 8n of the Cayley octonions
   (up to isomorphism).  The sequence is defined by:
      a(1) = 0,
      a(6) = 2, a(12) = 2,
      a(30) = 3,
      otherwise a(n) = 1.  */
seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)0;
    if (n == 6 || n == 12)
        return (seq_t)2;
    if (n == 30)
        return (seq_t)3;
    return (seq_t)1;
}

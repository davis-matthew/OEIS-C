/*
 * A095029  An example of a (v,k,lambda)=(21,5,1) cyclic difference set.
 * https://oeis.org/A095029
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   a(n) = the n-th element of the cyclic difference set D =
 *            {3,6,7,12,14} for n = 1..5; a(n) = 0 for all other n
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* There are only five terms in the sequence.  For n <= 5 we return the
       corresponding element of the difference set, otherwise we return 0. */
    if (n == 0)
        return (seq_t)0;
    if (n <= 5) {
        static const seq_t D[5] = { 3, 6, 7, 12, 14 };
        return D[n - 1];
    }
    return (seq_t)0;
}

/*
 * A095898  The (1,1)-term of the 3 X 3 matrix M^n, where M = [1,2,3 / 4,7,11
 *          / 6,10,16].
 * https://oeis.org/A095898
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 24*a(n-1) + a(n-2) for n>=3, a(1) = 1, a(2) = 27
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)   return (seq_t)1;          /* a(1) */
    if (n == 2)   return (seq_t)27;         /* a(2) */

    seq_t prevprev = (seq_t)1;              /* a(1) */
    seq_t prev     = (seq_t)27;             /* a(2) */
    seq_t cur = 0;
    for (idx_t i = 3; i <= n; ++i) {
        cur = (seq_t)24 * prev + prevprev;
        prevprev = prev;
        prev = cur;
    }
    return cur;
}

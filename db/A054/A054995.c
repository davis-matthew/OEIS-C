/*
 * A054995  A version of Josephus problem: a(n) is the surviving integer under
 *          the following elimination process. Arrange 1,2,3,...,n in a
 *          circle, increasing clockwise. Starting with i=1, delete the
 *          integer two places clockwise from i. Repeat, counting two places
 *          from the next undeleted integer, until only o...
 * https://oeis.org/A054995
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; for n > 1, a(n) = (a(n-1)+2) mod n + 1
 * Range:     u8 n <= 78, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the survivor in a Josephus problem with step size 3. */

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;
    seq_t res = (seq_t)1;        /* a(1) */
    for (idx_t i = 2; i <= n; ++i)
    {
        res = ((res + (seq_t)2) % (seq_t)i) + (seq_t)1;
    }
    return res;
}

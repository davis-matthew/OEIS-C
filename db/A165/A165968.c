/*
 * A165968  Number of pairings disjoint to a given pairing, and containing a
 *          given pair not in the given pairing.
 * https://oeis.org/A165968
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2 n − 4)·a(n−1) + (2 n − 6)·a(n−2) for n>2, with a(1)=0,
 *            a(2)=1. Implements OEIS A165968
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;
    if (n == 2)
        return (seq_t)1;

    seq_t a_nm2 = (seq_t)0;   /* a(1) */
    seq_t a_nm1 = (seq_t)1;   /* a(2) */
    seq_t a_n   = (seq_t)0;

    for (idx_t i = 3; i <= n; ++i)
    {
        a_n = (seq_t)(2 * i - 4) * a_nm1 + (seq_t)(2 * i - 6) * a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_nm1;
}

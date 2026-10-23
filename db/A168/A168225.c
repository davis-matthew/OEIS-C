/*
 * A168225  a(n) = n^4*(n^7 + 1)/2.
 * https://oeis.org/A168225
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^4*(n^7 + 1)/2
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 7, u64 n <= 60, u128 n <= 3390,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast index to seq_t for all arithmetic */
    seq_t n_seq = (seq_t)n;

    /* precompute powers */
    seq_t n2  = n_seq * n_seq;          /* n^2   */
    seq_t n4  = n2 * n2;                /* n^4   */
    seq_t n6  = n4 * n2;                /* n^6   */
    seq_t n7  = n6 * n_seq;             /* n^7   */
    seq_t n7p1 = n7 + 1;                /* n^7+1 */

    /* the product n^4*(n^7+1) is always even, so divide one factor by 2 */
    seq_t result;
    if ((n4 & 1) == 0)
    {
        /* n^4 is even */
        seq_t n4_half = n4 / 2;
        result = n4_half * n7p1;
    }
    else
    {
        /* n^7+1 is even when n is odd */
        seq_t n7p1_half = n7p1 / 2;
        result = n4 * n7p1_half;
    }

    return result;
}

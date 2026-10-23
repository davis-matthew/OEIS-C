/*
 * A187362  Pell trisection: Pell(3*n+2), n >= 0.
 * https://oeis.org/A187362
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 14*a(n-1) + a(n-2) a(-1) = 1, a(0) = 2
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)2;          /* a(0) */
    seq_t prevprev = (seq_t)1;    /* a(-1) */
    seq_t prev = (seq_t)2;        /* a(0) */
    for (idx_t i = (idx_t)1; i <= n; ++i)
    {
        seq_t curr = (seq_t)14 * prev + prevprev;
        prevprev = prev;
        prev = curr;
    }
    return prev;                  /* a(n) */
}

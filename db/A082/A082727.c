/*
 * A082727  a(0)=1, a(n)=abs(n-2*a(n-1)).
 * https://oeis.org/A082727
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = abs(n - 2*a(n-1)), a(0) = 1 */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;          /* a(0) = 1 */

    seq_t prev = (seq_t)1;        /* a(0) */

    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t i_seq   = (seq_t)i;          /* convert index to seq_t */
        seq_t two_prev = prev + prev;      /* 2 * a(i-1) */
        seq_t diff = (i_seq >= two_prev) ? (i_seq - two_prev)
                                          : (two_prev - i_seq);
        prev = diff;                       /* a(i) becomes prev for next iteration */
    }

    return prev;        /* a(n) */
}

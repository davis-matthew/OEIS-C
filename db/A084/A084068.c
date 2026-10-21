/*
 * A084068  a(1) = 1, a(2) = 2; a(2*k) = 2*a(2*k-1) - a(2*k-2), a(2*k+1) =
 *          4*a(2*k) - a(2*k-1).
 * https://oeis.org/A084068
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 26, u64 n <= 51, u128 n <= 101,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula (recurrence):
 *   a(1) = 1, a(2) = 2
 *   a(2k)   = 2*a(2k-1) - a(2k-2)
 *   a(2k+1) = 4*a(2k)   - a(2k-1)
 * This generates the A084068 sequence.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)1) return (seq_t)1;
    if (n == (idx_t)2) return (seq_t)2;

    seq_t a_im2 = (seq_t)1;   /* a(1) */
    seq_t a_im1 = (seq_t)2;   /* a(2) */
    for (idx_t i = 3; i <= n; ++i)
    {
        seq_t cur;
        if ((i & (idx_t)1) == (idx_t)1)   /* odd index */
            cur = (seq_t)4 * a_im1 - a_im2;
        else                              /* even index */
            cur = (seq_t)2 * a_im1 - a_im2;
        a_im2 = a_im1;
        a_im1 = cur;
    }
    return a_im1;
}

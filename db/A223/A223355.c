/*
 * A223355  Rolling cube footprints: number of n X 6 0..5 arrays starting with
 *          0 where 0..5 label faces of a cube and every array movement to a
 *          horizontal or antidiagonal neighbor moves across a corresponding
 *          cube edge.
 * https://oeis.org/A223355
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1600*a(n-1) - 368640*a(n-2) + 21233664*a(n-3) (n ≥ 4)
 *            Initial values: a(1) = 1024, a(2) = 1327104, a(3) = 1764753408
 * Range:     u8 none, u16 n <= 1, u32 n <= 3, u64 n <= 6, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1u)
        return (seq_t)1024;
    if (n == 2u)
        return (seq_t)1327104;
    if (n == 3u)
        return (seq_t)1764753408;

    /* prev1 = a[n-1], prev2 = a[n-2], prev3 = a[n-3] */
    seq_t prev1 = (seq_t)1764753408; /* a3 */
    seq_t prev2 = (seq_t)1327104;    /* a2 */
    seq_t prev3 = (seq_t)1024;       /* a1 */

    for (idx_t i = 4; i <= n; ++i)
    {
        seq_t cur = (seq_t)1600    * prev1
                    - (seq_t)368640 * prev2
                    + (seq_t)21233664 * prev3;

        /* shift the window */
        prev3 = prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}

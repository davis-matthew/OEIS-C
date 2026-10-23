/*
 * A188155  Number of 10-step self-avoiding walks on an n X n square summed
 *          over all starting positions.
 * https://oeis.org/A188155
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 16268*n^2 - 115548*n + 186528 for n > 8. For n = 1…8 the
 *            values are taken from the OEIS list
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t tbl[9] = {
        /* a(0) unused */ 0,
        0, 0, 0,          /* n=1,2,3  */
        3696,             /* n=4   */
        26000,            /* n=5   */
        82032,            /* n=6   */
        175312,           /* n=7   */
        303328            /* n=8   */
    };

    if (n <= 8)            /* 1 ≤ n ≤ 8 */
        return tbl[n];

    seq_t nn = n;
    return (seq_t)16268 * nn * nn - (seq_t)115548 * nn + (seq_t)186528;
}

/*
 * A151889  a(1)=2, a(2)=3; a(2k-1)=2a(2k-2)+a(2k-3), a(2k)=3a(2k-2)+2a(2k-3),
 *          k >= 2.
 * https://oeis.org/A151889
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=2, a(2)=3; For k >= 2: a(2k-1) = 2*a(2k-2) + a(2k-3), a(2k)
 *            = 3*a(2k-2) + 2*a(2k-3);
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)3;

    idx_t maxK = (n + 1) / 2;          /* k for the largest term needed */

    seq_t prev_prev = (seq_t)2;        /* a(1)   */
    seq_t prev      = (seq_t)3;        /* a(2)   */
    seq_t cur1, cur2;

    for (idx_t k = 2; k <= maxK; ++k) {
        cur1 = (seq_t)2 * prev + prev_prev;          /* a(2k-1) */
        cur2 = (seq_t)3 * prev + (seq_t)2 * prev_prev;/* a(2k)   */

        if (k == maxK) {
            return (n & 1) ? cur1 : cur2;   /* n odd -> cur1, even -> cur2 */
        }

        prev_prev = cur1;
        prev = cur2;
    }

    /* should never reach here */
    return (seq_t)0;
}

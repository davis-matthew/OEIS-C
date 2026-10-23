/*
 * A156156  a(n) = 6*a(n-1)-a(n-2) for n > 2; a(1) = 13, a(2) = 53.
 * https://oeis.org/A156156
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) - a(n-2), a(1) = 13, a(2) = 53
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1, behavior for n=0 not used */
    if (n == 1) return 13;
    if (n == 2) return 53;

    seq_t prevprev = 13;           /* a(1)   */
    seq_t prev     = 53;           /* a(2)   */
    seq_t cur      = 0;            /* a(n)   */

    for (idx_t i = 3; i <= n; ++i) {
        cur = 6 * prev - prevprev;
        prevprev = prev;
        prev = cur;
    }

    return cur;
}

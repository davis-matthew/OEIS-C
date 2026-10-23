/*
 * A176387  y-values in the solution to x^2-74*y^2=1.
 * https://oeis.org/A176387
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7398*a(n-1) - a(n-2) with a(1) = 0, a(2) = 430
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 6, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the base cases */
    if (n == 1u) return (seq_t)0;
    if (n == 2u) return (seq_t)430;

    /* iterate recurrence */
    seq_t prevprev = (seq_t)0;          /* a(1) */
    seq_t prev     = (seq_t)430;        /* a(2) */
    for (idx_t i = 3u; i <= n; ++i) {
        seq_t cur = (seq_t)7398 * prev - prevprev;
        prevprev = prev;
        prev     = cur;
    }
    return prev;
}

/*
 * A038762  a(n) = 6*a(n-1) - a(n-2) for n >= 2, with a(0)=3, a(1)=13.
 * https://oeis.org/A038762
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 6*a(n-1) - a(n-2), a(0)=3, a(1)=13
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the initial two terms directly. */
    if (n == 0) return (seq_t)3;
    if (n == 1) return (seq_t)13;

    /* Iterate using the recurrence; no dynamic allocation is needed. */
    seq_t prev_prev = (seq_t)3;  /* a(0) */
    seq_t prev      = (seq_t)13; /* a(1) */
    seq_t six = (seq_t)6;
    for (idx_t i = 2; i <= n; ++i) {
        seq_t cur = prev * six - prev_prev; /* a(i) = 6*a(i-1) - a(i-2) */
        prev_prev = prev;
        prev = cur;
    }
    return prev;
}

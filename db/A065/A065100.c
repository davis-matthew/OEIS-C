/*
 * A065100  a(n+2) = 9*a(n+1) - a(n), a(0) = 3, a(1) = 27.
 * https://oeis.org/A065100
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 9*a(n-1) - a(n-2); a(0) = 3, a(1) = 27
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)3;
    if (n == 1) return (seq_t)27;

    seq_t a_prev = (seq_t)3;
    seq_t a_curr = (seq_t)27;
    seq_t a_next;

    for (idx_t i = 2; i <= n; ++i) {
        a_next = (seq_t)(9 * a_curr - a_prev);
        a_prev = a_curr;
        a_curr = a_next;
    }
    return a_curr;
}

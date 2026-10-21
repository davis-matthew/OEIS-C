/*
 * A060645  a(0) = 0, a(1) = 4, then a(n) = 18*a(n-1) - a(n-2).
 * https://oeis.org/A060645
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 0, a(1) = 4, a(n) = 18*a(n-1) - a(n-2) for n >= 2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    if (n == 1)
        return (seq_t)4;

    seq_t a_prev2 = (seq_t)0;   /* a(0) */
    seq_t a_prev1 = (seq_t)4;   /* a(1) */
    seq_t a_curr;

    for (idx_t i = 2; i <= n; ++i) {
        a_curr = (seq_t)18 * a_prev1 - a_prev2;
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }
    return a_curr;
}

/*
 * A123004  Expansion of g.f. x^2/(1 - 2*x - 25*x^2).
 * https://oeis.org/A123004
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 26, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 2*a(n-1) + 25*a(n-2)
 * with initial values a(1) = 0, a(2) = 1
 */
seq_t a(idx_t n)
{
    /* a(1) is 0, a(2) is 1 by definition */
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;

    /* iterate using two successive terms */
    seq_t a_prevprev = (seq_t)0;   /* a(1) */
    seq_t a_prev     = (seq_t)1;   /* a(2) */
    seq_t a_curr     = 0;

    for (idx_t i = 3; i <= n; ++i) {
        a_curr = (seq_t)2 * a_prev + (seq_t)25 * a_prevprev;
        a_prevprev = a_prev;
        a_prev     = a_curr;
    }
    return a_curr;
}

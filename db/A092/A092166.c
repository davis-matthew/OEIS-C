/*
 * A092166  Let M = 2 X 2 matrix [ 1 2 / 5 4 ]; a(n) = (2,1) entry of M^n.
 * https://oeis.org/A092166
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) + 6*a(n-2) (for n ≥ 3), with a(1) = 5 and a(2) =
 *            25
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Undefined for n < 1 – return 0 to keep the type safe. */
    if (n < 1) return (seq_t)0;

    /* Base cases.  These are returned directly. */
    if (n == 1) return (seq_t)5;
    if (n == 2) return (seq_t)25;

    /* Iterate the recurrence.  Only the two most recent
       values are needed, so we keep them in local variables. */
    seq_t a_prev2 = (seq_t)5;    /* a(1) */
    seq_t a_prev1 = (seq_t)25;   /* a(2) */
    seq_t a_cur;

    for (idx_t i = 3; i <= n; ++i) {
        a_cur = a_prev1 * 5 + a_prev2 * 6;
        a_prev2 = a_prev1;
        a_prev1 = a_cur;
    }
    return a_prev1;
}

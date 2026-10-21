/*
 * A227111  Nonnegative solutions of the Pell equation x^2 - 89*y^2 = +1.
 *          Solutions y = 53000*a(n).
 * https://oeis.org/A227111
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=1, a(n)=1000002*a(n-1)-a(n-2) (n>=2) where 1000002
 *            = 2*500001
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 4, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation evaluates the linear recurrence in O(n) time
   with no external dependencies.  It works for any seq_t/idx_t
   combination that supports the required arithmetic operators. */
seq_t a(idx_t n)
{
    /* trivial cases */
    if (n == 0) {
        return 0;
    }
    if (n == 1) {
        return 1;
    }

    /* iterate the recurrence */
    seq_t prev2 = 0;                          /* a(n-2)   */
    seq_t prev1 = 1;                          /* a(n-1)   */
    seq_t curr;                               /* a(n)     */
    const seq_t M = (seq_t)1000002;           /* recurrence coefficient */

    for (idx_t i = 2; i <= n; ++i) {
        curr = M * prev1 - prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}

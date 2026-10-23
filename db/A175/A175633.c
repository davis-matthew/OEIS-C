/*
 * A175633  Numbers x such that x^2 - 28*y^2 = 1 for some integer y.
 * https://oeis.org/A175633
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 254*a(n-1) - a(n-2), a(1)=1, a(2)=127
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* OEIS offset starts at 1; return 0 for an invalid index. */
    if (n == 0)
        return (seq_t)0;

    if (n == 1)
        return (seq_t)1;

    if (n == 2)
        return (seq_t)127;

    /* Use two previous terms only: a(n-2) and a(n-1). */
    seq_t a_prev2 = (seq_t)1;      /* a(1) */
    seq_t a_prev1 = (seq_t)127;    /* a(2) */

    for (idx_t i = 3; i <= n; ++i) {
        seq_t a_curr = ((seq_t)254 * a_prev1) - a_prev2;
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }

    return a_prev1;
}

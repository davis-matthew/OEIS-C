/*
 * A095940  a(n+2) = 5*a(n+1) - 3*a(n) (n >= 1); a(0) = 0, a(1) = 1, a(2) = 4.
 * https://oeis.org/A095940
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+2) = 5*a(n+1) - 3*a(n), with a(0)=0, a(1)=1, a(2)=4
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)4;

    seq_t prev2 = (seq_t)1;   /* a(1) */
    seq_t prev1 = (seq_t)4;   /* a(2) */
    seq_t curr  = prev1;

    for (idx_t i = 3; i <= n; ++i) {
        curr = (seq_t)(5 * prev1 - 3 * prev2);
        prev2 = prev1;
        prev1 = curr;
    }
    return curr;
}

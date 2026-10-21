/*
 * A054492  a(n) = 3*a(n-1) - a(n-2), a(0)=1, a(1)=6.
 * https://oeis.org/A054492
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*a(n-1) - a(n-2), a(0) = 1, a(1) = 6
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 22, u64 n <= 45, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the two initial values */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)6;

    /* Iterate the recurrence up to n */
    seq_t prev2 = (seq_t)1; /* a(n-2) */
    seq_t prev1 = (seq_t)6; /* a(n-1) */
    seq_t curr  = 0;        /* a(n) */

    for (idx_t i = 2; i <= n; ++i) {
        /* curr = 3*prev1 - prev2 */
        curr = (seq_t)3 * prev1 - prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return curr;
}

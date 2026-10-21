/*
 * A210731  a(n) = a(n-1) + a(n-2) + n + 3 with n>1, a(0) = a(1) = 0.
 * https://oeis.org/A210731
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + n + 3, a(0)=0, a(1)=0
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 42, u64 n <= 88, u128 n <= 181,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the two known initial values explicitly */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)0;

    /* Use two previous terms to iterate up to n */
    seq_t a_prev2 = (seq_t)0;   /* a(0) */
    seq_t a_prev1 = (seq_t)0;   /* a(1) */
    seq_t a_curr;

    for (idx_t i = 2; i <= n; ++i) {
        a_curr = a_prev1 + a_prev2 + (seq_t)i + (seq_t)3;
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }
    return a_prev1;
}

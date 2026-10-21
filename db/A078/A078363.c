/*
 * A078363  A Chebyshev T-sequence with Diophantine property.
 * https://oeis.org/A078363
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 2, a(1) = 13, a(n) = 13*a(n-1) - a(n-2) for n >= 2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first two terms explicitly. */
    if (n == (idx_t)0) return (seq_t)2;
    if (n == (idx_t)1) return (seq_t)13;

    /* Use a simple iterative recurrence.  For n up to 10 000 this is
       easily fast enough in all supported builds. */
    seq_t a_prev2 = (seq_t)2;   /* a(0) */
    seq_t a_prev1 = (seq_t)13;  /* a(1) */

    for (idx_t i = (idx_t)2; i <= n; ++i) {
        seq_t a_curr = (seq_t)13 * a_prev1 - a_prev2;
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }

    return a_prev1;
}

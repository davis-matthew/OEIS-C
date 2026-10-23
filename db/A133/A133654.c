/*
 * A133654  a(n) = 2*A000129(n) - 1.
 * https://oeis.org/A133654
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + a(n-2) + 2, a(1) = 1, a(2) = 3
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 25, u64 n <= 50, u128 n <= 101,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the trivial cases that lie before the recurrence starts */
    if (n == 1) {
        return (seq_t)1;
    }
    if (n == 2) {
        return (seq_t)3;
    }

    /* Use two variables to hold the two last computed terms.
       a_prev1 stores a(n-2), a_prev2 stores a(n-1). */
    seq_t a_prev1 = (seq_t)1;   /* a(1) */
    seq_t a_prev2 = (seq_t)3;   /* a(2) */
    seq_t a_curr  = 0;          /* placeholder for a(n) */

    /* Compute successive terms up to a(n). */
    for (idx_t i = 3; i <= n; ++i) {
        a_curr = (seq_t)2 * a_prev2 + a_prev1 + (seq_t)2;
        a_prev1 = a_prev2;
        a_prev2 = a_curr;
    }

    return a_curr;
}

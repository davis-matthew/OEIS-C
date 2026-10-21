/*
 * A210680  a(n) = (3*a(n-1)) XOR a(n-2).
 * https://oeis.org/A210680
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (3*a(n-1)) XOR a(n-2), a(0) = 0, a(1) = 1
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 41, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the base cases directly. */
    if (n == 0)
        return 0;
    if (n == 1)
        return 1;

    /* Keep the two most recent values. */
    seq_t a_prev2 = 0;   /* a(n-2)  */
    seq_t a_prev1 = 1;   /* a(n-1)  */
    seq_t a_curr;

    /* Iterate from 2 up to n, computing the next value each time. */
    for (idx_t i = 2; i <= n; ++i)
    {
        a_curr = ((((seq_t)3) * a_prev1) ^ a_prev2);
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }

    return a_prev1;
}

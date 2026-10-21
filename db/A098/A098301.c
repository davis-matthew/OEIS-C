/*
 * A098301  Member r=16 of the family of Chebyshev sequences S_r(n) defined in
 *          A092184.
 * https://oeis.org/A098301
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 0, a(1) = 1, a(n) = 14*a(n-1) - a(n-2) + 2 (n ≥ 2)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first two indices directly. */
    if (n == 0)
        return (seq_t)0;
    if (n == 1)
        return (seq_t)1;

    /* Iteratively compute using the recurrence, keeping only the last two values. */
    seq_t a_prev2 = (seq_t)0;      /* a(0)   */
    seq_t a_prev1 = (seq_t)1;      /* a(1)   */
    seq_t a_curr  = a_prev1;       /* placeholder */

    for (idx_t i = 2; i <= n; ++i)
    {
        a_curr = (seq_t)14 * a_prev1 - a_prev2 + (seq_t)2;
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }

    return a_curr;
}

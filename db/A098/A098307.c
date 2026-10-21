/*
 * A098307  Unsigned member r=-7 of the family of Chebyshev sequences S_r(n)
 *          defined in A092184.
 * https://oeis.org/A098307
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*a(n-1) - a(n-2) + 2*(-1)^(n+1), with a(0) = 0, a(1) = 1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle small indices directly */
    if (n == (idx_t)0) return (seq_t)0;
    if (n == (idx_t)1) return (seq_t)1;

    /* Iteratively compute up to n. */
    seq_t a_prev2 = (seq_t)0;   /* a(0) */
    seq_t a_prev1 = (seq_t)1;   /* a(1) */
    seq_t a_curr = a_prev1;     /* placeholder for current term */

    for (idx_t i = (idx_t)2; i <= n; ++i)
    {
        /* a(i) = 9*a(i-1) - a(i-2) + 2*(-1)^(i+1) */
        a_curr = a_prev1 * (seq_t)9;
        a_curr -= a_prev2;
        if ((i + (idx_t)1) & (idx_t)1)
        {
            /* (-1)^(i+1) = -1 for odd (i+1), so subtract 2 */
            a_curr -= (seq_t)2;
        }
        else
        {
            /* add 2 for even (i+1) */
            a_curr += (seq_t)2;
        }

        /* Shift indices for next iteration */
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }

    return a_curr;
}

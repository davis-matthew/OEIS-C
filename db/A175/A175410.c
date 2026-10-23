/*
 * A175410  a(n) = (b(m)-1)*b(m) = Sum_{n=b(m)+1,...,c(m)}n, b=A046174,
 *          c=A046175, m=n+1.
 * https://oeis.org/A175410
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (b(n+1)-1) * b(n+1) * where b(m) are the integer
 *            solutions of * u^2 - 3*v^2 = -2 with u ≡ 5 (mod 6), * b = (u +
 *            1) / 6. * The pair (u, v) follows the recurrence * u_{k+1} =
 *            2*u_k + 3*v_k * v_{k+1} = u_k + 2*v_k * with u_0 = 1, v_0 = 1. *
 *            The k-th odd index k = 2*m - 1 gives b(m) for m >= 2...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* u, v grow following the Pell recurrence */
    seq_t u = 1;          /* u_0 */
    seq_t v = 1;          /* v_0 */
    idx_t   k = 0;        /* current index in the Pell sequence */
    idx_t   i = 0;        /* number of a terms produced so far */

    for (;;)
    {
        /* When k is odd and >= 3 we obtain a new term */
        if ((k & 1) && k >= 3)
        {
            seq_t b = (u + 1) / 6;           /* b(m) */
            seq_t a_val = (b - 1) * b;       /* a(n) */
            ++i;
            if ((idx_t)i == n)
                return a_val;
        }

        /* Advance to the next (u, v) pair */
        seq_t new_u = u * 2 + v * 3;
        seq_t new_v = u + v * 2;
        u = new_u;
        v = new_v;
        ++k;
    }
}

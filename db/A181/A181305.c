/*
 * A181305  Number of increasing columns in all 2-compositions of n.
 * https://oeis.org/A181305
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) satisfies the linear recurrence a(n) = 7*a(n-1) - 12*a(n-2)
 *            - 4*a(n-3) + 12*a(n-4) - 4*a(n-5), with initial values a(0)=0,
 *            a(1)=1, a(2)=5, a(3)=24, a(4)=104. This recurrence follows from
 *            the generating function z*(1-z)^2/((1+z)*(1-4*z+2*z^2)^2)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Initial values up to a(4) */
    seq_t v[5] = {0, 1, 5, 24, 104};

    if (n < 5)
        return v[n];

    for (idx_t i = 5; i <= n; ++i)
    {
        /* Compute a(i) from the recurrence */
        seq_t term = 7 * v[4] + 12 * v[1];
        term -= (12 * v[3] + 4 * v[2] + 4 * v[0]);

        /* Shift the window of previous 5 terms */
        v[0] = v[1];
        v[1] = v[2];
        v[2] = v[3];
        v[3] = v[4];
        v[4] = term;
    }

    return v[4];
}

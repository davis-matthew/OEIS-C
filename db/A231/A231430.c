/*
 * A231430  Number of ternary sequences which contain 000.
 * https://oeis.org/A231430
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 2*(3^(n-4) - a(n-4)) Initial values: a(0)=0,
 *            a(1)=0, a(2)=0, a(3)=1
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle trivial indices directly */
    if (n == 0 || n == 1 || n == 2)
        return (seq_t)0;
    if (n == 3)
        return (seq_t)1;

    /* Set up initial values for the sliding window */
    seq_t a_n_minus4 = (seq_t)0;  /* a(0) */
    seq_t a_n_minus3 = (seq_t)0;  /* a(1) */
    seq_t a_n_minus2 = (seq_t)0;  /* a(2) */
    seq_t a_n_minus1 = (seq_t)1;  /* a(3) */

    /* pow3 holds 3^(i-4) during the loop */
    seq_t pow3 = (seq_t)1;        /* 3^(4-4) initially */

    for (idx_t i = 4; i <= n; ++i)
    {
        /* Compute the next term */
        seq_t term = 3 * a_n_minus1 + 2 * (pow3 - a_n_minus4);

        /* Shift the window for the next iteration */
        a_n_minus4 = a_n_minus3;
        a_n_minus3 = a_n_minus2;
        a_n_minus2 = a_n_minus1;
        a_n_minus1 = term;

        /* Update pow3 for the next index (i+1) */
        pow3 *= (seq_t)3;
    }

    return a_n_minus1;
}

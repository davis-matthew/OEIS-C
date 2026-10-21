/*
 * A096424  a(1)=a(2)=1, a(n) = (a(n-2)+F(n-2)) * (a(n-1)+F(n-1)) for n > 2
 *          where F(i) is the i-th Fibonacci no.
 * https://oeis.org/A096424
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 8, u64 n <= 9, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * a(1) = a(2) = 1
 * a(n) = (a(n-2) + F(n-2)) * (a(n-1) + F(n-1))  for n > 2
 * where F(i) is the ith Fibonacci number with F(1)=1, F(2)=1.
 */
seq_t a(idx_t n)
{
    /* Handle the offset: n >= 1 */
    if (n == 0)
        return (seq_t)0;               /* undefined below offset */
    if (n == 1 || n == 2)
        return (seq_t)1;

    /* Variables to hold the two most recent terms of the sequence */
    seq_t a_prev = (seq_t)1;      /* a(n-1) initially a(2) */
    seq_t a_prevprev = (seq_t)1;  /* a(n-2) initially a(1) */

    /* Variables to hold the two most recent Fibonacci numbers
     * F(2) = F(1) = 1 initially for indices 1 and 2. */
    seq_t f_prev = (seq_t)1;      /* F(n-1) initially F(2) */
    seq_t f_prevprev = (seq_t)1;  /* F(n-2) initially F(1) */

    /* We will iterate from i = 3 up to n */
    for (idx_t i = 3; i <= n; ++i)
    {
        /* Compute the current Fibonacci number F(i) = F(i-1) + F(i-2) */
        seq_t f_cur = f_prev + f_prevprev;

        /* Compute a(i) = (a(n-2)+F(n-2)) * (a(n-1)+F(n-1)) */
        seq_t term = (a_prevprev + f_prevprev) * (a_prev + f_prev);

        /* Shift the state for the next iteration */
        a_prevprev = a_prev;
        a_prev = term;

        f_prevprev = f_prev;
        f_prev = f_cur;
    }

    /* After the loop, a_prev holds a(n) */
    return a_prev;
}

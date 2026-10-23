/*
 * A024188  a(n) = ((n+2)!/2)(1/3 - 1/4 + ... + c/(n+2)), where c =
 *          (-1)^(n+1).
 * https://oeis.org/A024188
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((n+2)!/2)(1/3 - 1/4 + … + c/(n+2)), where c = (-1)^(n+1)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1 – below values do not matter */
    if (n == 1) return 1;

    seq_t fact   = 2;              /* 2!  (used for n=1, incremented in the loop) */
    seq_t a_prev = 1;              /* a(1) */

    for (idx_t i = 2; i <= n; ++i)
    {
        /* Update factorial to (i+1)! */
        fact = fact * (seq_t)(i + 1);

        /* term = ((i+1)!)/2  (always divisible by 2) */
        seq_t term = fact / 2;

        /* a(i) = (i+2) * a_prev  +/- ((i+1)!)/2   */
        seq_t a_new = ((seq_t)(i + 2)) * a_prev;

        /* sign = (-1)^(i+1)  -> negative when (i+1) is odd */
        if ((i + 1) & 1)
            a_new -= term;          /* subtract when sign is negative */
        else
            a_new += term;          /* add when sign is positive */

        a_prev = a_new;             /* prepare for next iteration */
    }

    return a_prev;                  /* a(n) */
}

/*
 * A076453  a(n+2) = abs(a(n+1)) - a(n), a(0)=1, a(1)=0.
 * https://oeis.org/A076453
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n+2) = abs(a(n+1)) - a(n), a(0) = 1, a(1) = 0
 * Range:     i8 n <= 99, i16 n <= 99, i32 n <= 99, i64 n <= 99, i128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Period of length 9 for any initial pair (a(0), a(1)) */
    static seq_t period[9];
    static int init = 0;

    if (!init)
    {
        period[0] = 1;
        period[1] = 0;
        /* Compute remaining terms of the period */
        for (idx_t i = 2; i < 9; ++i)
        {
            seq_t absval = period[i - 1] >= 0 ? period[i - 1] : -period[i - 1];
            period[i] = absval - period[i - 2];
        }
        init = 1;
    }

    /* Return the term at index n within the 9‑term period */
    return period[n % 9];
}

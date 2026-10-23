/*
 * A105751  Imaginary part of Product_{k=0..n} (1 + k*i), i = sqrt(-1).
 * https://oeis.org/A105751
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = ((2*n-1)*a(n-1)-(n^2-2*n+2)*n*a(n-2))/(n-1) for n>1,
 *            a(0)=0, a(1)=1. This implementation uses a 128‑bit intermediate
 *            when seq_t is 64‑bit to avoid overflow of the products. For
 *            arbitrary‑precision seq_t the normal operators are used and all
 *            computations are exact
 * Range:     i8 n <= 5, i16 n <= 7, i32 n <= 12, i64 n <= 20, i128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    if (n == 1)
        return (seq_t)1;

    seq_t a_prevprev = (seq_t)0;   /* a(n-2)  */
    seq_t a_prev     = (seq_t)1;   /* a(n-1)  */
    seq_t a_curr     = a_prev;     /* placeholder */

    for (idx_t i = 2; i <= n; ++i)
    {
        /* For 64‑bit seq_t use 128‑bit intermediates.  For larger
           seq_t sizes the normal big‑int operators are safe.   */
        if (sizeof(seq_t) <= 8)
        {
            __int128 ii          = i;
            __int128 term1       = (2*ii - 1) * (__int128)a_prev;
            __int128 factor      = (ii*ii - 2*ii + 2) * ii;  /* (n^2-2n+2)*n */
            __int128 term2       = factor * (__int128)a_prevprev;
            __int128 num         = term1 - term2;
            a_curr              = (seq_t)(num / (ii - 1));
        }
        else
        {
            /* Compute (n^2-2n+2) exactly using 128‑bit for the
               intermediate that fits into a 64‑bit counter.     */
            seq_t factor = (seq_t)(((__int128)i*i - 2*i + 2));
            seq_t factor_i = factor * (seq_t)i;               /* factor * n */
            seq_t term1    = (seq_t)(2*i - 1) * a_prev;
            seq_t term2    = factor_i * a_prevprev;
            seq_t num      = term1 - term2;
            a_curr         = num / (seq_t)(i - 1);
        }

        /* Shift for next iteration */
        a_prevprev = a_prev;
        a_prev     = a_curr;
    }

    return a_curr;
}

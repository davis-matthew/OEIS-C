/*
 * A169236  Number of reduced words of length n in Coxeter group on 31
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^28 = I.
 * https://oeis.org/A169236
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 For n >= 1: a(n) = 29 * sum_{k=1}^{27} a(n-k) - 435 *
 *            a(n-28) + b(n) where b( n ) = 1 if n = 0 or 28 2 if 1 <= n <= 27
 *            0 otherwise (These values come from the numerator of the
 *            rational generating function given in the OEIS entry.)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 28‑entry circular buffer for the last computed values */
    seq_t buf[28] = {0};
    seq_t sum27      = 0;   /* sum of the last up to 27 values */
    seq_t a_n        = 0;   /* value for the current n */
    idx_t i, pos;

    for (i = 0; i <= n; ++i)
    {
        /* b(n) */
        seq_t b;
        if (i == 0)              b = 1;
        else if (i == 28)        b = 1;
        else if (i <= 27)        b = 2;
        else                     b = 0;

        /* a(n) = 29*sum27 - 435*a(n-28) + b  */
        seq_t temp = ((seq_t)29) * sum27 + b;
        seq_t sub  = ((seq_t)435) *
                ((i >= 28) ? buf[(i - 28) % 28] : (seq_t)0);
        a_n = temp - sub;      /* result is non‑negative */

        /* store a(n) */
        pos = i % 28;
        buf[pos] = a_n;

        /* update sum27 for the next step */
        sum27 += a_n;                     /* add the new term   */
        if (i >= 27)                      /* remove the term that is now 28 positions back */
            sum27 -= buf[(i - 27) % 28];
    }

    return a_n;
}

/*
 * A093963  Antidiagonal sums of array in A093966.
 * https://oeis.org/A093963
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   2*a(n) -5*a(n-1) -(n+2)*a(n-2) +2*(n+6)*a(n-3) +(n-13)*a(n-4)
 *            -4*(n-3)*a(n-5) +2*(n-3)*a(n-6) = 0 with
 *            a(1)=1,a(2)=3,a(3)=8,a(4)=20,a(5)=49,a(6)=123
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 20, u64 n <= 36, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The routine evaluates the recurrence iteratively, keeping only the six
   most recent terms.  It works for any seq_t that supports the usual
   arithmetic operators (+,-,*,/, two‑sided with integers).  No dynamic
   allocation is needed. */

seq_t a(idx_t n)
{
    if (n == 0)                           /* behaviour below offset is
                                            unspecified */
        return 0;

    /* initial values, a(1) … a(6) */
    static const seq_t init[6] = {(seq_t)1,(seq_t)3,(seq_t)8,
                                   (seq_t)20,(seq_t)49,(seq_t)123};

    if (n <= 6)
        return init[n - 1];

    /* a1 … a6 correspond to a(n-6) … a(n-1) */
    seq_t a1 = init[0], a2 = init[1], a3 = init[2];
    seq_t a4 = init[3], a5 = init[4], a6 = init[5];

    for (idx_t i = 7; i <= n; ++i)
    {
        /* compute 2*a(i) using the recurrence */
        seq_t term = 5 * a6                                        /* 5*a(n-1) */
                  + ((seq_t)(i + 2)) * a5                         /* (i+2)*a(n-2) */
                  - 2 * ((seq_t)(i + 6)) * a4                    /* -2*(i+6)*a(n-3) */
                  - ((seq_t)((long long)i - 13)) * a3            /* -(i-13)*a(n-4) */
                  + 4 * ((seq_t)(i - 3)) * a2                    /* +4*(i-3)*a(n-5) */
                  - 2 * ((seq_t)(i - 3)) * a1;                   /* -2*(i-3)*a(n-6) */

        term = term / 2;   /* a(i) = (2*a(i))/2 */

        /* shift the sliding window for the next iteration */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = a6;
        a6 = term;
    }

    return a6;   /* a(n) */
}

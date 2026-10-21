/*
 * A234265  Number of (n+1) X (7+1) 0..2 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 2
 *          (constant-stress 1 X 1 tilings).
 * https://oeis.org/A234265
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 6*a(n-2) - 24*a(n-3) + 4*a(n-4) + 36*a(n-5) -
 *            24*a(n-6) (n >= 7) with initial values a(1)=3104, a(2)=3746,
 *            a(3)=4924, a(4)=7304, a(5)=11876, a(6)=21164
 * Range:     u8 none, u16 n <= 7, u32 n <= 22, u64 n <= 47, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The recurrence is empirical; it holds for all n >= 7. */

seq_t a(idx_t n)
{
    /* first six terms known from the OEIS data */
    static const seq_t init[6] = {
        3104ULL,   /* a(1) */
        3746ULL,   /* a(2) */
        4924ULL,   /* a(3) */
        7304ULL,   /* a(4) */
        11876ULL,  /* a(5) */
        21164ULL   /* a(6) */
    };

    /* For n <= 6 simply return the stored value */
    if (n <= 6)
        return init[n - 1];

    /* r[0] … r[5] hold a(n-6) … a(n-1) during the forward sweep */
    seq_t r[6];
    r[0] = init[0];   /* a(1) */
    r[1] = init[1];   /* a(2) */
    r[2] = init[2];   /* a(3) */
    r[3] = init[3];   /* a(4) */
    r[4] = init[4];   /* a(5) */
    r[5] = init[5];   /* a(6) */

    /* Compute successive values from n = 7 up to the requested index */
    for (idx_t i = 7; i <= n; ++i)
    {
        /* Positive contributions */
        seq_t pos = r[5] * 3 + r[4] * 6 + r[2] * 4 + r[1] * 36;
        /* Negative contributions (subtracted later) */
        seq_t neg = r[3] * 24 + r[0] * 24;
        seq_t new_val = pos - neg;

        /* Shift the window: a(n-6) … a(n-1) become a(n-5) … a(n) */
        r[0] = r[1];
        r[1] = r[2];
        r[2] = r[3];
        r[3] = r[4];
        r[4] = r[5];
        r[5] = new_val;
    }

    return r[5];   /* a(n) */
}

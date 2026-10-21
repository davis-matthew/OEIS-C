/*
 * A231452  Number of (2+1)X(n+1) 0..3 arrays with no element unequal to a
 *          strict majority of its horizontal and antidiagonal neighbors, with
 *          values 0..3 introduced in row major order.
 * https://oeis.org/A231452
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) -7*a(n-2) -20*a(n-3) +52*a(n-4) -14*a(n-5)
 *            -69*a(n-6) +56*a(n-7) +24*a(n-8) +14*a(n-9) -81*a(n-10)
 *            +112*a(n-12) -6*a(n-13) -3*a(n-14) -36*a(n-15) -27*a(n-16), with
 *            initial values a(1)=6, a(2)=15, a(3)=32, a(4)=87, a(5)=229,
 *            a(6)=647, a(7)=1829, a(8)=5293, a(9)=15405, a(10)=45267,...
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Initial 16 terms of the sequence */
    static const seq_t init[17] = {
        (seq_t)0,   (seq_t)6,    (seq_t)15,   (seq_t)32,   (seq_t)87,
        (seq_t)229, (seq_t)647,  (seq_t)1829, (seq_t)5293, (seq_t)15405,
        (seq_t)45267,(seq_t)133622,(seq_t)396229,(seq_t)1178280,
        (seq_t)3512183,(seq_t)10486280,(seq_t)31348643
    };

    if (n <= 16u)
        return init[n];

    /* Coefficients of the recurrence, index 1..16 */
    static const int coeff[17] = {
        0,  /* dummy 0 index */
        6, -7, -20, 52, -14, -69, 56, 24, 14, -81, 0,
        112, -6, -3, -36, -27
    };

    /* Circular buffer holding the last 16 terms:
       last[0] = a(k-1), last[1] = a(k-2), … , last[15] = a(k-16)    */
    typedef uint8_t uint8; /* dummy to keep ANSI C happy if needed */
    seq_t last[16];
    for (int i = 0; i < 16; ++i)
        last[i] = init[16 - i];   /* last[0]=a(16), last[1]=a(15), … */

    seq_t res = 0;
    for (idx_t k = 17; k <= n; ++k)
    {
        seq_t sumPos = 0, sumNeg = 0;
        for (int d = 1; d <= 16; ++d)
        {
            int c = coeff[d];
            unsigned int m = c > 0 ? (unsigned int)c : (unsigned int)(-c);
            seq_t term = last[d - 1] * m;
            if (c > 0)
                sumPos += term;
            else
                sumNeg += term;
        }
        res = sumPos - sumNeg;        /* final value a(k) */

        /* shift buffer: drop oldest, insert new at front */
        for (int i = 15; i >= 1; --i)
            last[i] = last[i - 1];
        last[0] = res;
    }

    return res;
}

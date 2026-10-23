/*
 * A020970  Expansion of 1/((1-7*x)*(1-9*x)*(1-10*x)).
 * https://oeis.org/A020970
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 26*a(n-1) - 223*a(n-2) + 630*a(n-3), n>=3; a(0) = 1, a(1)
 *            = 26, a(2) = 453
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 26;
    if (n == 2) return 453;

    seq_t a0 = 1;      /* a(n-3)  */
    seq_t a1 = 26;     /* a(n-2)  */
    seq_t a2 = 453;    /* a(n-1)  */
    seq_t an = 0;      /* placeholder for a(n) */

    for (idx_t i = 3; i <= n; ++i) {
        /* Compute a(n) = 26*a(n-1) - 223*a(n-2) + 630*a(n-3) */
        seq_t t1 = 26 * a2;   /* 26*a(n-1)   */
        seq_t t2 = 223 * a1;  /* 223*a(n-2)  */
        seq_t t3 = 630 * a0;  /* 630*a(n-3)  */

        an = t1 + t3;         /* addition modulo 2^bits      */
        an -= t2;             /* subtraction modulo 2^bits   */

        /* shift the window */
        a0 = a1;
        a1 = a2;
        a2 = an;
    }

    return an;
}

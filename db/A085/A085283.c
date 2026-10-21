/*
 * A085283  a(n) = n*n^n - (n-1)*(n-1)^n.
 * https://oeis.org/A085283
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*n^n - (n-1)*(n-1)^n (with a(0) = 1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp)
    {
        if (exp & 1)
            result = result * base;      /* multiplication works for built‑in or
                                            * arbitrary‑precision types          */
        base = base * base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;                 /* a(0) = 1 */

    seq_t sn   = (seq_t)n;               /* n as seq_t           */
    seq_t snm1 = (seq_t)(n - 1);         /* n-1 as seq_t        */

    seq_t pow_n   = pow_seq_t(sn,   n);  /* n^n                  */
    seq_t pow_nm1 = pow_seq_t(snm1, n);  /* (n-1)^n              */

    seq_t term1 = sn * pow_n;            /* n * n^n              */
    seq_t term2 = snm1 * pow_nm1;        /* (n-1)*(n-1)^n        */

    return term1 - term2;                /* a(n)                  */
}

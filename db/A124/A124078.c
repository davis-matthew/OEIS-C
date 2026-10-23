/*
 * A124078  a(n) = H(n)*n!/(floor(n/2))!, where H(n) = Sum_{k=1..n} 1/k, the
 *          n-th harmonic number.
 * https://oeis.org/A124078
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = H(n) * n! / (floor(n/2))!
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 16, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where H(n) = Sum_{k=1..n} 1/k,   a(0)=0 */

seq_t a(idx_t n)
{
    /* r = floor(n/2), n!/(r!) called f, H(n) depends on n;
       we maintain f and a incrementally. */

    if (n == 0)
        return (seq_t)0;      /* a(0) = 0 */

    seq_t f = (seq_t)1;      /* f(0) = 1  */
    seq_t a_val = (seq_t)0;  /* a(0) = 0  */

    for (idx_t i = 1; i <= n; ++i)
    {
        /* f(i-1) is stored in f before we update it */
        if (i & 1)          /* i is odd  */
        {
            seq_t prev_f = f;
            f = f * (seq_t)i;          /* f(i) = f(i-1) * i   */
            a_val = a_val * (seq_t)i + prev_f; /* a(i) = a(i-1)*i + f(i-1) */
        }
        else                /* i is even */
        {
            seq_t prev_f = f;
            f = f * (seq_t)2;          /* f(i) = f(i-1) * 2   */
            seq_t half = (seq_t)(i / 2); /* i/2 is integer  */
            a_val = a_val * (seq_t)2 + prev_f / half; /* a(i) = a(i-1)*2 + f(i-1)/(i/2) */
        }
    }

    return a_val;
}

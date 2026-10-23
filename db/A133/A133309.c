/*
 * A133309  a(n) = (1/n)*Sum_{i=0..n-1} C(n,i)*C(n,i+1)*8^i*9^(n-i), a(0)=1.
 * https://oeis.org/A133309
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=9; for n ≥ 2, (n+1) * a(n) = 17 * (2*n - 1) *
 *            a(n-1) - (n-2) * a(n-2)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)9;

    seq_t a_prev = (seq_t)1;  /* a(0) */
    seq_t a_curr = (seq_t)9;  /* a(1) */

    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t k = (seq_t)(17 * (2 * i - 1));      /* 17*(2*i-1) */
        seq_t term1 = a_curr * k;                 /* 17*(2*i-1)*a(n-1) */
        seq_t term2 = a_prev * (seq_t)(i - 2);    /* (n-2)*a(n-2)   */
        seq_t numer = term1 - term2;              /* numerator      */
        a_prev = a_curr;
        a_curr = numer / (seq_t)(i + 1);          /* a(n) = numer/(n+1) */
    }

    return a_curr;
}

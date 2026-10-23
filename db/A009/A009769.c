/*
 * A009769  Expansion of tanh(log(1+1/x)).
 * https://oeis.org/A009769
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -2*n*a(n-1)-2*n*(n-1)*a(n-2), a(0)=1, a(1)=0
 * Range:     i8 n <= 5, i16 n <= 6, i32 n <= 10, i64 n <= 18, i128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if ((idx_t)0 == n) return (seq_t)1;
    if ((idx_t)1 == n) return (seq_t)0;

    seq_t a0 = (seq_t)1; /* a(0) */
    seq_t a1 = (seq_t)0; /* a(1) */

    for (idx_t i = (idx_t)2; i <= n; ++i)
    {
        seq_t bi   = (seq_t)i;
        seq_t mul1 = (seq_t)2 * bi;          /* 2*i          */
        seq_t mul2 = (seq_t)2 * bi * (bi - (seq_t)1); /* 2*i*(i-1) */
        seq_t ai   = -(mul1 * a1 + mul2 * a0);

        a0 = a1;
        a1 = ai;
    }
    return a1;
}

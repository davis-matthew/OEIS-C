/*
 * A021524  Expansion of 1/((1-x)*(1-3*x)*(1-6*x)*(1-11*x)).
 * https://oeis.org/A021524
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 17*a(n-1) - 66*a(n-2) + (3^(n+1)-1)/2 for n>1, with a(0)
 *            = 1 and a(1) = 21
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)21;

    seq_t a0 = (seq_t)1;      /* a(n-2)  */
    seq_t a1 = (seq_t)21;     /* a(n-1)  */
    seq_t cur3pow = (seq_t)27;/* 3^(i+1) for i=2   */

    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t part = (cur3pow - (seq_t)1) / (seq_t)2;   /* (3^(i+1)-1)/2   */
        seq_t next = (seq_t)17 * a1 - (seq_t)66 * a0 + part;
        a0 = a1;
        a1 = next;
        cur3pow = cur3pow * (seq_t)3;   /* advance to 3^(i+2) */
    }
    return a1;
}

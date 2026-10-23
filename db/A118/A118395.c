/*
 * A118395  Expansion of e.g.f. exp(x + x^3).
 * https://oeis.org/A118395
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 23, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A118395: Expansion of e.g.f. exp(x + x^3).
   Formula used: 
      a(0)=1, a(1)=1, a(2)=1
      a(n) = a(n-1) + 3*(n-1)*(n-2)*a(n-3)   for n >= 3
   Computes the value for any n >= 0; does not depend on any external
   libraries and works with arbitrary-precision seq_t or native unsigned
   integers.
*/
seq_t a(idx_t n)
{
    if (n <= 2)
        return (seq_t)1;

    seq_t a0 = (seq_t)1;   /* a(n-3) */
    seq_t a1 = (seq_t)1;   /* a(n-2) */
    seq_t a2 = (seq_t)1;   /* a(n-1) */

    for (idx_t i = 3; i <= n; ++i)
    {
        /* term = a2 + 3*(i-1)*(i-2)*a0 */
        seq_t term = a2 + ((seq_t)3 * (seq_t)(i-1) * (seq_t)(i-2) * a0);

        /* shift for next iteration: a0<-a1, a1<-a2, a2<-term */
        a0 = a1;
        a1 = a2;
        a2 = term;
    }

    return a2;      /* a(n) */
}

/*
 * A094120  a(n) = Sum_{k=1..n} Sum_{i=1..k} (-2)^valuation(i,2).
 * https://oeis.org/A094120
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} Sum_{i=1..k} (-2)^{valuation(i,2)}
 * Range:     u8 n <= 30, u16 n <= 510, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute (-2)^{valuation(i,2)} by determining the exponent of 2 in i (v2(i)),
   then multiplying 2^e by (-1)^e.  Incrementally maintain the inner sum
   f(k)=Sum_{i=1..k} (-2)^{v2(i)} and the outer sum a(n)=Sum_{k=1..n} f(k).  */
seq_t a(idx_t n)
{
    seq_t inner_sum = 0;     /* f(k) */
    seq_t outer_sum = 0;     /* a(n) */

    for (idx_t i = 1; i <= n; ++i) {
        /* compute exponent of 2 in i (v2) */
        idx_t tmp = i;
        int v2 = 0;
        while ((tmp & 1) == 0) {  /* tmp is even */
            ++v2;
            tmp >>= 1;
        }

        /* term = (-2)^v2 = (-1)^v2 * 2^v2 */
        seq_t pow2 = ((seq_t)1) << v2;          /* 2^v2 */
        seq_t term = (v2 & 1) ? -pow2 : pow2;  /* apply sign */

        inner_sum += term;
        outer_sum += inner_sum;
    }

    return outer_sum;
}

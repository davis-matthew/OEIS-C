/*
 * A051524  Second unsigned column of triangle A051338.
 * https://oeis.org/A051524
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0, a(1)=1;
   a(n) = (2*n+9)*a(n-1) - (n+4)^2*a(n-2)   (for n>=2) */

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;
    if (n == (idx_t)1) return (seq_t)1;

    seq_t prev2 = (seq_t)0;      /* a(0) */
    seq_t prev1 = (seq_t)1;      /* a(1) */
    seq_t cur   = (seq_t)0;

    for (idx_t i = (idx_t)2; i <= n; ++i)
    {
        seq_t coeff1 = (seq_t)(2 * i + 9);
        seq_t coeff2 = (seq_t)(i + 4);

        /* a(i) = coeff1 * a(i-1) - coeff2^2 * a(i-2) */
        seq_t term1 = coeff1 * prev1;
        seq_t term2 = coeff2 * coeff2 * prev2;
        cur = term1 - term2;

        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}

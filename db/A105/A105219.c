/*
 * A105219  a(n) = Sum_{k=0..n} C(n,k)^2*(n-k)!*k^2.
 * https://oeis.org/A105219
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence implementation:
   a(0)=0, a(1)=1, a(2)=8
   a(n) = 2*(n+2)*a(n-1) - (n^2+4*n-4)*a(n-2) + 2*(n-2)*(n-1)*a(n-3) */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)8;

    seq_t a0 = (seq_t)0;   /* a(n-3) */
    seq_t a1 = (seq_t)1;   /* a(n-2) */
    seq_t a2 = (seq_t)8;   /* a(n-1) */
    seq_t aCur = (seq_t)0;

    for (idx_t i = 3; i <= n; ++i) {
        /* 2*(i+2)*a(n-1) */
        seq_t term1 = (seq_t)2 * ((seq_t)i + (seq_t)2) * a2;

        /* (i^2 + 4*i - 4)*a(n-2) */
        seq_t coeff = (seq_t)(i * i + 4 * i - 4);
        seq_t term2 = coeff * a1;

        /* 2*(i-2)*(i-1)*a(n-3) */
        seq_t term3 = (seq_t)2 * ((seq_t)(i - 2)) * ((seq_t)(i - 1)) * a0;

        aCur = term1 - term2 + term3;

        /* shift window for next iteration */
        a0 = a1;
        a1 = a2;
        a2 = aCur;
    }

    return aCur;
}

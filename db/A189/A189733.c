/*
 * A189733  Denominator of B(0,n) where B(n,n)=0, B(n-1,n) = (-1)^(n+1)/n, and
 *          B(m,n) = B(m+1,n-1) + B(m,n-1), n >= 0, m >= 0, is an array of
 *          fractions.
 * https://oeis.org/A189733
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = 1 if n%6==0 or 4 a(n) = 1 + 3*floor(n/6) if n%6==1 a(n) =
 *            1 + 2*floor(n/6) if n%6==2 a(n) = 2 + 3*floor(n/6) if n%6==3
 *            a(n) = 6 + 6*floor(n/6) if n%6==5 (equivalently, for n≥12: a(n)
 *            = 2*a(n-6) - a(n-12))
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t k = n / 6;              /* floor(n/6) */
    switch (n % 6) {
        case 0:   return (seq_t)1;
        case 1:   return (seq_t)1 + (seq_t)3 * (seq_t)k;
        case 2:   return (seq_t)1 + (seq_t)2 * (seq_t)k;
        case 3:   return (seq_t)2 + (seq_t)3 * (seq_t)k;
        case 4:   return (seq_t)1;
        case 5:   return (seq_t)6 + (seq_t)6 * (seq_t)k;
        default: return (seq_t)0;   /* unreachable, for completeness */
    }
}

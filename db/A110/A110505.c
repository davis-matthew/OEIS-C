/*
 * A110505  Numerators of unsigned columns of triangle A110504: a(n) =
 *          n!*A110504(n,0) = (-1)^k*n!*A110504(n+k,k) for all k >= 0.
 * https://oeis.org/A110505
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  frac, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: 4*a(n) = 2*(2*n-1)*a(n-1) + (n-2)*(n+1)*a(n-2)
                  - (n-3)*(n-2)*n*a(n-3)  for n>=4, with
   a(0)=0, a(1)=1, a(2)=3, a(3)=7 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;
    if (n == 3) return (seq_t)7;

    seq_t a_m3 = (seq_t)1;   /* a(n-3) = a(1) */
    seq_t a_m2 = (seq_t)3;   /* a(n-2) = a(2) */
    seq_t a_m1 = (seq_t)7;   /* a(n-1) = a(3) */

    for (idx_t i = 4; i <= n; ++i) {
        seq_t i_seq = (seq_t)i;

        seq_t term1 = ((seq_t)2 * ((seq_t)2 * i_seq - (seq_t)1)) * a_m1;
        seq_t term2 = ((i_seq - (seq_t)2) * (i_seq + (seq_t)1)) * a_m2;
        seq_t term3 = ((i_seq - (seq_t)3) * (i_seq - (seq_t)2) * i_seq) * a_m3;

        seq_t num = term1 + term2;
        num -= term3;               /* term3 < term1+term2 for all valid n */
        seq_t next = num / (seq_t)4;

        a_m3 = a_m2;
        a_m2 = a_m1;
        a_m1 = next;
    }

    return a_m1;
}

/*
 * A052674  Expansion of e.g.f. (1-x)/(1-3*x-2*x^2+2*x^3).
 * https://oeis.org/A052674
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=2, a(2)=16,
   a(n) = 3*n*a(n-1) + 2*n*(n-1)*a(n-2) - 2*n*(n-1)*(n-2)*a(n-3)   (n≥3) */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)16;

    seq_t a0 = (seq_t)1;  /* a(n-3) */
    seq_t a1 = (seq_t)2;  /* a(n-2) */
    seq_t a2 = (seq_t)16; /* a(n-1) */
    seq_t an;

    for (idx_t i = 3; i <= n; ++i) {
        seq_t i_seq = (seq_t)i;
        seq_t term1 = (seq_t)3 * i_seq * a2;
        seq_t term2 = (seq_t)2 * i_seq * (i_seq - (seq_t)1) * a1;
        seq_t term3 = (seq_t)2 * i_seq * (i_seq - (seq_t)1) * (i_seq - (seq_t)2) * a0;
        an = term1 + term2 - term3;
        a0 = a1;
        a1 = a2;
        a2 = an;
    }

    return a2;
}

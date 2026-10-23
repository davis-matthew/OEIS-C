/*
 * A162921  Number of reduced words of length n in Coxeter group on 4
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^4 = I.
 * https://oeis.org/A162921
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=4, a(2)=12, a(3)=36, a(4)=102,
 *            a(n)=2*a(n-1)+2*a(n-2)+2*a(n-3)-3*a(n-4) for n≥5
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 20, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)12;
    if (n == 3) return (seq_t)36;
    if (n == 4) return (seq_t)102;

    seq_t a1 = (seq_t)4;    /* a(1) */
    seq_t a2 = (seq_t)12;   /* a(2) */
    seq_t a3 = (seq_t)36;   /* a(3) */
    seq_t a4 = (seq_t)102;  /* a(4) */
    seq_t next;
    for (idx_t i = 5; i <= n; ++i) {
        next = (seq_t)2 * a4 + (seq_t)2 * a3 + (seq_t)2 * a2 - (seq_t)3 * a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = next;
    }
    return a4;
}

/*
 * A162740  Number of reduced words of length n in Coxeter group on 4
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^3 = I.
 * https://oeis.org/A162740
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=4, a(2)=12, a(3)=30; a(n) = 2*a(n-1) + 2*a(n-2) -
 *            3*a(n-3) for n>3
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 25, u64 n <= 52, u128 n <= 105,
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
    if (n == 3) return (seq_t)30;

    seq_t a3 = (seq_t)30; /* a(n-1)  */
    seq_t a2 = (seq_t)12; /* a(n-2)  */
    seq_t a1 = (seq_t)4;  /* a(n-3)  */

    for (idx_t i = 4; i <= n; ++i) {
        seq_t next = a3 * ((seq_t)2) + a2 * ((seq_t)2) - a1 * ((seq_t)3);
        /* shift for next iteration */
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}

/*
 * A162767  Number of reduced words of length n in Coxeter group on 12
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^3 = I.
 * https://oeis.org/A162767
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=12, a(2)=132, a(3)=1386, and for n≥4:
 *            a(n)=10*a(n-1)+10*a(n-2)-55*a(n-3)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first four terms directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)12;
    if (n == 2) return (seq_t)132;
    if (n == 3) return (seq_t)1386;

    /* 3 previous values: a(n-3), a(n-2), a(n-1) */
    seq_t a_nm3 = (seq_t)12;   /* a(1) */
    seq_t a_nm2 = (seq_t)132;  /* a(2) */
    seq_t a_nm1 = (seq_t)1386; /* a(3) */
    seq_t a_n   = 0;           /* will hold a(n) in each iteration */

    for (idx_t i = 4; i <= n; ++i)
    {
        a_n   = (seq_t)10 * a_nm1 + (seq_t)10 * a_nm2 - (seq_t)55 * a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}

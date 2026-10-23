/*
 * A162920  Number of reduced words of length n in Coxeter group on 3
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^4 = I.
 * https://oeis.org/A162920
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)+a(n-2)+a(n-3)-a(n-4) for n >= 5, with initial
 *            values a(0)=1, a(1)=3, a(2)=6, a(3)=12, a(4)=21
 * Range:     u8 n <= 8, u16 n <= 18, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the offset region directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)6;
    if (n == 3) return (seq_t)12;
    if (n == 4) return (seq_t)21;

    /* sliding window: a_{n-4}, a_{n-3}, a_{n-2}, a_{n-1} */
    seq_t a_nminus4 = (seq_t)3;   /* a(1) */
    seq_t a_nminus3 = (seq_t)6;   /* a(2) */
    seq_t a_nminus2 = (seq_t)12;  /* a(3) */
    seq_t a_nminus1 = (seq_t)21;  /* a(4) */

    for (idx_t i = 5; i <= n; ++i) {
        seq_t next = a_nminus1 + a_nminus2 + a_nminus3;
        next -= a_nminus4;     /* guaranteed non‑negative */
        a_nminus4 = a_nminus3;
        a_nminus3 = a_nminus2;
        a_nminus2 = a_nminus1;
        a_nminus1 = next;
    }
    return a_nminus1;
}

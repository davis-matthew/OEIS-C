/*
 * A025992  Expansion of 1/((1-2*x)*(1-5*x)*(1-7*x)*(1-8*x)).
 * https://oeis.org/A025992
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 22*a(n-1) - 171*a(n-2) + 542*a(n-3) - 560*a(n-4); initial
 *            values: a(0)=1, a(1)=22, a(2)=313, a(3)=3666;
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)22;
    if (n == 2) return (seq_t)313;
    if (n == 3) return (seq_t)3666;

    seq_t a0 = (seq_t)1;    /* a(n-4)  */
    seq_t a1 = (seq_t)22;   /* a(n-3)  */
    seq_t a2 = (seq_t)313;  /* a(n-2)  */
    seq_t a3 = (seq_t)3666; /* a(n-1)  */

    for (idx_t i = 4; i <= n; ++i) {
        /* compute a(n) = 22*a(n-1) - 171*a(n-2) + 542*a(n-3) - 560*a(n-4) */
        seq_t t1 = (seq_t)22 * a3 + (seq_t)542 * a1;
        seq_t t2 = (seq_t)171 * a2 + (seq_t)560 * a0;
        seq_t an = t1 - t2;          /* guaranteed non‑negative */

        /* shift window for next iteration */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return a3;
}

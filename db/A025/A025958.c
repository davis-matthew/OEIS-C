/*
 * A025958  Expansion of 1/((1-2*x)*(1-4*x)*(1-5*x)*(1-6*x)).
 * https://oeis.org/A025958
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 17*a(n-1)-104*a(n-2)+268*a(n-3)-240*a(n-4), a(0)=1,
 *            a(1)=17, a(2)=185, a(3)=1645
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 22, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)17;
    if (n == 2) return (seq_t)185;
    if (n == 3) return (seq_t)1645;

    seq_t a0 = (seq_t)1;     /* a(n-4)  */
    seq_t a1 = (seq_t)17;    /* a(n-3)  */
    seq_t a2 = (seq_t)185;   /* a(n-2)  */
    seq_t a3 = (seq_t)1645;  /* a(n-1)  */

    for (idx_t i = 4; i <= n; ++i) {
        seq_t an = (seq_t)17 * a3
                 - (seq_t)104 * a2
                 + (seq_t)268 * a1
                 - (seq_t)240 * a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }

    return a3;
}

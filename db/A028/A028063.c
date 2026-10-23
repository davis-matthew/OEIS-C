/*
 * A028063  Expansion of 1/((1-3*x)*(1-5*x)*(1-7*x)*(1-12*x)).
 * https://oeis.org/A028063
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 27*a(n-1) - 251*a(n-2) + 957*a(n-3) - 1260*a(n-4) with
 *            a(0)=1, a(1)=27, a(2)=478, a(3)=7086
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)27;
    if (n == 2) return (seq_t)478;
    if (n == 3) return (seq_t)7086;

    seq_t a0 = (seq_t)1;      /* a(n-3)  */
    seq_t a1 = (seq_t)27;     /* a(n-2)  */
    seq_t a2 = (seq_t)478;    /* a(n-1)  */
    seq_t a3 = (seq_t)7086;   /* a(n)    */
    seq_t an = a3;

    for (idx_t i = 4; i <= n; ++i) {
        an = 27 * a3 - 251 * a2 + 957 * a1 - 1260 * a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return an;
}

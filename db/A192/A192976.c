/*
 * A192976  Coefficient of x in the reduction by x^2 -> x+1 of the polynomial
 *          p(n,x) defined at Comments.
 * https://oeis.org/A192976
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 5*a(n-2) + a(n-3) + 2*a(n-4) - a(n-5), a(0)=0,
 *            a(1)=1, a(2)=2, a(3)=10, a(4)=29
 * Range:     u8 n <= 6, u16 n <= 17, u32 n <= 40, u64 n <= 86, u128 n <= 178,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first five values explicitly */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)10;
    if (n == 4) return (seq_t)29;

    /* a0..a4 hold the last five computed values:
         a0 = a(n-5), a1 = a(n-4), a2 = a(n-3), a3 = a(n-2), a4 = a(n-1)
    */
    seq_t a0 = (seq_t)0;
    seq_t a1 = (seq_t)1;
    seq_t a2 = (seq_t)2;
    seq_t a3 = (seq_t)10;
    seq_t a4 = (seq_t)29;
    seq_t ai;

    for (idx_t i = 5; i <= n; ++i) {
        ai = 4*a4 - 5*a3 + a2 + 2*a1 - a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = ai;
    }

    return a4;
}

/*
 * A099448  A Chebyshev transform of A030191 associated to the knot 7_6.
 * https://oeis.org/A099448
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) satisfies a(0)=1, a(1)=5, a(2)=19, a(3)=65,
 *            a(n)=5*a(n-1)-7*a(n-2)+5*a(n-3)-a(n-4) for n>=4 (G.f.:
 *            (1+x²)/(1-5x+7x²-5x³+x⁴) )
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 18, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* initial values for n = 0..3 */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;
    if (n == 2) return (seq_t)19;
    if (n == 3) return (seq_t)65;

    /* constants */
    const seq_t five   = 5;
    const seq_t seven  = 7;

    /* iterate recurrence up to n */
    seq_t a0 = 1;  /* a(n-4) */
    seq_t a1 = 5;  /* a(n-3) */
    seq_t a2 = 19; /* a(n-2) */
    seq_t a3 = 65; /* a(n-1) */
    idx_t i;

    for (i = 4; i <= n; ++i) {
        seq_t a4 = a3 * five - a2 * seven + a1 * five - a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
    }
    return a3;
}

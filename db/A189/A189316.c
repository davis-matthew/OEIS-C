/*
 * A189316  Expansion of g.f. 5*(1-x-x^2)/((1+x)*(1-3*x+x^2)).
 * https://oeis.org/A189316
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 2*a(n-2) - a(n-3), n>2, a(0)=5, a(1)=5,
 *            a(2)=15
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 22, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0 || n == 1) return 5;
    if (n == 2) return 15;

    seq_t a0 = 5;   /* a(n-3) */
    seq_t a1 = 5;   /* a(n-2) */
    seq_t a2 = 15;  /* a(n-1) */
    seq_t an;

    for (idx_t i = 3; i <= n; ++i) {
        an = 2 * a2 + 2 * a1 - a0;
        a0 = a1;
        a1 = a2;
        a2 = an;
    }
    return a2;
}

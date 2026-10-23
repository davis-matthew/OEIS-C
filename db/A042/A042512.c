/*
 * A042512  Numerators of continued fraction convergents to sqrt(785).
 * https://oeis.org/A042512
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 56*a(n-1) + a(n-2), a(0) = 28, a(1) = 1569
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 4, u64 n <= 10, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)28;
    if (n == 1) return (seq_t)1569;

    seq_t a0 = (seq_t)28;          /* a(n-2)  */
    seq_t a1 = (seq_t)1569;        /* a(n-1)  */
    seq_t an;                     /* a(n)    */

    for (idx_t i = 2; i <= n; ++i) {
        an = (seq_t)56 * a1 + a0;
        a0 = a1;
        a1 = an;
    }
    return an;
}

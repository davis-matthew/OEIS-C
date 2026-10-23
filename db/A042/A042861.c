/*
 * A042861  Denominators of continued fraction convergents to sqrt(962).
 * https://oeis.org/A042861
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 62*a(n-1) + a(n-2); a(0)=1, a(1)=62
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)62;

    seq_t a0 = 1;   /* a(0) */
    seq_t a1 = 62;  /* a(1) */
    seq_t an;

    for (idx_t i = 2; i <= n; ++i) {
        an = 62 * a1 + a0;
        a0 = a1;
        a1 = an;
    }
    return a1;
}

/*
 * A036243  Denominator of fraction equal to the continued fraction [ 0, 2, 4,
 *          ...2n ].
 * https://oeis.org/A036243
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = 2*n*a(n-1) + a(n-2), a(0)=1, a(1)=2
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)2;

    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)2;   /* a(1) */
    seq_t an;

    for (idx_t i = 2; i <= n; ++i) {
        an = (seq_t)2 * (seq_t)i * a1 + a0;  /* a(i) = 2*i*a(i-1) + a(i-2) */
        a0 = a1;
        a1 = an;
    }
    return an;
}

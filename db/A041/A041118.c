/*
 * A041118  Numerators of continued fraction convergents to sqrt(68).
 * https://oeis.org/A041118
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(0)=8, a(1)=33; for n>=2: if n is odd : a(n)=4*a(n-1)+a(n-2) if
 *            n is even : a(n)=16*a(n-1)+a(n-2) (numerators of convergents to
 *            sqrt(68))
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)8;
    if (n == 1) return (seq_t)33;

    seq_t p0 = (seq_t)8;   /* a(0) */
    seq_t p1 = (seq_t)33;  /* a(1) */

    for (idx_t k = 2; k <= n; ++k) {
        seq_t coeff = (k & 1) ? (seq_t)4 : (seq_t)16; /* coefficient depends on parity of k */
        seq_t cur = coeff * p1 + p0;
        p0 = p1;
        p1 = cur;
    }
    return p1;
}

/*
 * A041481  Denominators of continued fraction convergents to sqrt(257).
 * https://oeis.org/A041481
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 32*a(n-1) + a(n-2) with a(0) = 1, a(1) = 32
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)32;

    seq_t a0 = (seq_t)1;   /* a(n-2) */
    seq_t a1 = (seq_t)32;  /* a(n-1) */
    seq_t an;

    for (idx_t i = 2; i <= n; ++i) {
        an = (seq_t)((seq_t)32 * a1 + a0);
        a0 = a1;
        a1 = an;
    }
    return a1;
}

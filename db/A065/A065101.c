/*
 * A065101  a(0) = c, a(1) = p*c^3; a(n+2) = p*c^2*a(n+1) - a(n), for p = 3, c
 *          = 2.
 * https://oeis.org/A065101
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(0) = 2, a(1) = 24, a(n+2) = 12*a(n+1) - a(n) */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)24;

    seq_t a0 = (seq_t)2;   /* a(0) */
    seq_t a1 = (seq_t)24;  /* a(1) */
    seq_t an;

    for (idx_t i = 2; i <= n; ++i) {
        an = a1 * (seq_t)12 - a0;
        a0 = a1;
        a1 = an;
    }
    return a1;
}

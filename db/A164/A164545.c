/*
 * A164545  a(n) = 4*a(n-1) + 4*a(n-2) for n > 1; a(0) = 1, a(1) = 8.
 * https://oeis.org/A164545
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) + 4*a(n-2); a(0) = 1, a(1) = 8
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)8;

    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)8;   /* a(1) */
    seq_t an = a1;

    for (idx_t i = 2; i <= n; ++i) {
        an = (seq_t)4 * a1 + (seq_t)4 * a0;
        a0  = a1;
        a1  = an;
    }
    return an;
}

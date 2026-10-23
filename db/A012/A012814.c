/*
 * A012814  Take every 5th term of Padovan sequence A000931, beginning with
 *          the third term.
 * https://oeis.org/A012814
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+3) = 5*a(n+2) - 4*a(n+1) + a(n), a(0)=0, a(1)=1, a(2)=5
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)5;

    seq_t a0 = 0, a1 = 1, a2 = 5, an;
    for (idx_t i = 3; i <= n; ++i) {
        an = 5 * a2 - 4 * a1 + a0;
        a0 = a1;
        a1 = a2;
        a2 = an;
    }
    return a2;
}

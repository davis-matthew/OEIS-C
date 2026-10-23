/*
 * A163068  a(n) = 16*a(n-1) - 59*a(n-2) for n > 1; a(0) = 2, a(1) = 21.
 * https://oeis.org/A163068
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16*a(n-1) - 59*a(n-2), with a(0)=2, a(1)=21
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 2;
    if (n == 1) return 21;
    seq_t a0 = 2;
    seq_t a1 = 21;
    seq_t an;
    for (idx_t i = 2; i <= n; ++i) {
        an = 16 * a1 - 59 * a0;
        a0 = a1;
        a1 = an;
    }
    return a1;
}

/*
 * A136252  a(n) = a(n-1) + 2*a(n-2) - 2*a(n-3).
 * https://oeis.org/A136252
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 2*a(n-2) - 2*a(n-3), a(0) = 1, a(1) = 3, a(2) =
 *            5
 * Range:     u8 n <= 12, u16 n <= 28, u32 n <= 60, u64 n <= 124,
 *            u128 n <= 252, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)5;

    seq_t a0 = 1, a1 = 3, a2 = 5;
    for (idx_t i = 3; i <= n; ++i) {
        seq_t an = a2 + (seq_t)2 * a1 - (seq_t)2 * a0;
        a0 = a1;
        a1 = a2;
        a2 = an;
    }
    return a2;
}

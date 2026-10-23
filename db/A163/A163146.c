/*
 * A163146  a(n) = 12*a(n-1)-31*a(n-2) for n > 1; a(0) = 1, a(1) = 11.
 * https://oeis.org/A163146
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*a(n-1)-31*a(n-2); a(0) = 1, a(1) = 11
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 11;
    seq_t a0 = 1, a1 = 11, a2;
    for (idx_t i = 2; i <= n; ++i) {
        a2 = (seq_t)12 * a1 - (seq_t)31 * a0;
        a0 = a1;
        a1 = a2;
    }
    return a1;
}

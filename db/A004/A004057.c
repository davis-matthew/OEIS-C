/*
 * A004057  Expansion of (1-x)/( (1+x)*(1-2*x)*(1-3*x)*(1-4*x)).
 * https://oeis.org/A004057
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) - 17*a(n-2) - 2*a(n-3) + 24*a(n-4), a(0) = 1,
 *            a(1) = 7, a(2) = 39, a(3) = 191
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t a0 = 1, a1 = 7, a2 = 39, a3 = 191;
    idx_t i;

    if (n == 0) return a0;
    if (n == 1) return a1;
    if (n == 2) return a2;
    if (n == 3) return a3;

    for (i = 4; i <= n; i++) {
        seq_t next = (seq_t)8 * a3 + (seq_t)24 * a0;
        next -= (seq_t)17 * a2;
        next -= (seq_t)2 * a1;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}

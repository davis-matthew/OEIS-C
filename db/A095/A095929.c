/*
 * A095929  Number of closed walks of length 2n at a vertex of the cyclic
 *          graph on 10 nodes C_10.
 * https://oeis.org/A095929
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1)-13*a(n-2)+4*a(n-3), with a(0) = 1, a(1) = 2,
 *            a(2) = 6
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)6;

    seq_t a0 = 1, a1 = 2, a2 = 6;   /* a0 = a(n-3), a1 = a(n-2), a2 = a(n-1) */

    for (idx_t i = 3; i <= n; ++i) {
        seq_t cur = 7 * a2 - 13 * a1 + 4 * a0;
        a0 = a1;
        a1 = a2;
        a2 = cur;
    }

    return a2;
}

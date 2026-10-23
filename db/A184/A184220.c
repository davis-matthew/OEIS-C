/*
 * A184220  a(n) = largest k such that A000290(n+1) = A000290(n) + (A000290(n)
 *          mod k), or 0 if no such k exists.
 * https://oeis.org/A184220
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0 for n <= 4; a(n) = (n-1)^2 - 2 for n >= 5
 * Range:     u8 n <= 17, u16 n <= 257, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 5) {
        return 0;
    }
    seq_t nn = n;
    seq_t t = nn - 1;
    return t * t - 2;
}

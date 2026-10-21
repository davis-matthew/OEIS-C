/*
 * A217571  a(n) = (2*n*(n+5) + (2*n+1)*(-1)^n - 1)/8.
 * https://oeis.org/A217571
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+6)/4 if n is even, else a(n) = (n*n + 4*n - 1)/4
 * Range:     u8 n <= 29, u16 n <= 509, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    if ((n & 1u) == 0u) {
        return nn * (nn + 6) / 4;
    } else {
        return ((nn * nn + 4 * nn) - (seq_t)1) / 4;
    }
}

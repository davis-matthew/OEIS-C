/*
 * A100542  Two-color Rado numbers R(0,n).
 * https://oeis.org/A100542
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)/2 for n > 3; a(1) = 5, a(2) = 1, a(3) = 9
 * Range:     u8 n <= 22, u16 n <= 361, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)5;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)9;
    seq_t tn = (seq_t)n;
    return tn * (tn + 1) / 2;
}

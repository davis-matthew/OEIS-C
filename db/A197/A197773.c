/*
 * A197773  Ceiling((n+1/n)^3).
 * https://oeis.org/A197773
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil((n + 1/n)^3)
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1625, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)8;
    if (n == 2) return (seq_t)16;
    if (n == 3) return (seq_t)38;

    seq_t N = (seq_t)n;
    seq_t n3 = N * N * N;
    return n3 + 3 * N + 1;
}

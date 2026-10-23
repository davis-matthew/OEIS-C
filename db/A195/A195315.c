/*
 * A195315  Centered 32-gonal numbers.
 * https://oeis.org/A195315
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16*n^2 - 16*n + 1
 * Range:     u8 n <= 4, u16 n <= 64, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    unsigned long long nn = (unsigned long long)n;
    unsigned long long val = 16ULL * nn * nn - 16ULL * nn + 1ULL;
    return (seq_t)val;
}

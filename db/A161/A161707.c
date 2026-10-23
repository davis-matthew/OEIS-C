/*
 * A161707  a(n) = (4*n^3 - 9*n^2 + 11*n + 3)/3.
 * https://oeis.org/A161707
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n^3 - 9*n^2 + 11*n + 3)/3
 * Range:     u8 n <= 6, u16 n <= 37, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    unsigned long long nn = (unsigned long long)n;
    unsigned long long val = (4ULL * nn * nn * nn
                            - 9ULL * nn * nn
                            + 11ULL * nn
                            + 3ULL) / 3ULL;
    return (seq_t)val;
}

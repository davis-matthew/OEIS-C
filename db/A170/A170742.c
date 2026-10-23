/*
 * A170742  Expansion of g.f.: (1+x)/(1-22*x).
 * https://oeis.org/A170742
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1, for n = 0 a(n) = 23 * 22^(n-1), for n > 0
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t result = (seq_t)23;
    for (idx_t i = 1; i < n; ++i)
        result *= (seq_t)22;

    return result;
}

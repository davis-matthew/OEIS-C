/*
 * A170759  Expansion of g.f.: (1+x)/(1-39*x).
 * https://oeis.org/A170759
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; a(1) = 40; a(n) = 39 * a(n-1) for n >= 2
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;
    if (n == 1)
        return 40;

    seq_t result = 40;
    for (idx_t i = 2; i <= n; ++i)
        result *= 39;
    return result;
}

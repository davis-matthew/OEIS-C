/*
 * A170736  Expansion of g.f.: (1+x)/(1-16*x).
 * https://oeis.org/A170736
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 17*16^(n-1) for n≥1; a(0) = 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    seq_t result = (seq_t)1;
    for (idx_t i = 1; i < n; ++i)
        result *= (seq_t)16;
    result *= (seq_t)17;
    return result;
}

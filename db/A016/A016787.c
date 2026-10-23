/*
 * A016787  a(n) = (3*n + 1)^11.
 * https://oeis.org/A016787
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n+1)^11
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 2, u64 n <= 18, u128 n <= 1060,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base   = ((seq_t)n) * (seq_t)3 + (seq_t)1;
    seq_t result = (seq_t)1;
    for (int i = 0; i < 11; ++i)
    {
        result = result * base;
    }
    return result;
}

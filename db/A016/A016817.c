/*
 * A016817  a(n) = (4n+1)^5.
 * https://oeis.org/A016817
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4n+1)^5
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 20, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base = ((seq_t)n * 4) + 1;
    seq_t result = base;
    result *= base;
    result *= base;
    result *= base;
    result *= base;
    return result;
}

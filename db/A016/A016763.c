/*
 * A016763  a(n) = (2*n+1)^11.
 * https://oeis.org/A016763
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n+1)^11
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 3, u64 n <= 27, u128 n <= 1591,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base = (seq_t)(2 * n + 1);
    seq_t result = 1;
    for (int k = 0; k < 11; ++k)
        result *= base;
    return result;
}

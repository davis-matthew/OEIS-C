/*
 * A017002  a(n) = (7*n + 1)^10.
 * https://oeis.org/A017002
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7*n+1)^10
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 11, u128 n <= 1018,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base = (seq_t)7 * (seq_t)n + (seq_t)1;
    seq_t result = 1;
    for (int i = 0; i < 10; ++i)
        result *= base;
    return result;
}

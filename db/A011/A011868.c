/*
 * A011868  a(n) = floor(n*(n-1)/15).
 * https://oeis.org/A011868
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*(n-1)/15)
 * Range:     u8 n <= 62, u16 n <= 991, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* n >= 1; convert to seq_t for safe multiplication, avoiding underflow */
    seq_t nn = (seq_t)n;
    return nn * (nn - 1) / (seq_t)15;
}

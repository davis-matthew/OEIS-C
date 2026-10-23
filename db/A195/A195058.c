/*
 * A195058  Concentric 23-gonal numbers.
 * https://oeis.org/A195058
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 23*n^2/4 + 19*((-1)^n - 1)/8
 * Range:     u8 n <= 6, u16 n <= 106, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Calculate 23*n^2/4 */
    seq_t part1 = (seq_t)23 * (seq_t)n * (seq_t)n / (seq_t)4;
    /* For odd n subtract 4 (the value of 19*((-1)^n-1)/8) */
    if (n & 1)
        return part1 - (seq_t)4;
    return part1;
}

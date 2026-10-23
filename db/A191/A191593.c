/*
 * A191593  Number of partitions of 12*n into parts < 5.
 * https://oeis.org/A191593
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*n^3 + 15*n^2 + 6*n + 1
 * Range:     u8 n <= 2, u16 n <= 17, u32 n <= 709, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t result = 12 * nn * nn * nn
                 + 15 * nn * nn
                 + 6  * nn
                 + 1;
    return result;
}

/*
 * A156796  a(n) = 81*n - 22.
 * https://oeis.org/A156796
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 81*n - 22
 * Range:     u8 n <= 3, u16 n <= 809, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (seq_t)n * (seq_t)81 - (seq_t)22;
}

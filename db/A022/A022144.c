/*
 * A022144  Coordination sequence for root lattice B_2.
 * https://oeis.org/A022144
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0^n + 8*n
 * Range:     u8 n <= 31, u16 n <= 8191, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    return (seq_t)8 * (seq_t)n;
}

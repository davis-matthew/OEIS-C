/*
 * A157621  a(n) = 625n^2 - 364n + 53.
 * https://oeis.org/A157621
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 625*n^2 - 364*n + 53
 * Range:     u8 none, u16 n <= 10, u32 n <= 2621, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t val = (seq_t)625 * nn * nn;
    val -= (seq_t)364 * nn;
    val += 53;
    return val;
}

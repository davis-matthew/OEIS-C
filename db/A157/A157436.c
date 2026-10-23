/*
 * A157436  a(n) = 128*n^2 + 2528*n + 12481.
 * https://oeis.org/A157436
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 128*n^2 + 2528*n + 12481. Offset: 1
 * Range:     u8 none, u16 n <= 12, u32 n <= 5782, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* out of range, offset is 1 */
    seq_t nn = (seq_t)n;
    return nn * nn * 128 + nn * 2528 + 12481;
}

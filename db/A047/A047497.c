/*
 * A047497  Numbers that are congruent to {1, 2, 4, 5, 7} mod 8.
 * https://oeis.org/A047497
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((8n-3)/5)
 * Range:     u8 n <= 160, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* the offset is 1; for n == 0 the sequence is undefined,
       but we return 0 safely. */
    if (n == 0) return 0;
    seq_t sn = (seq_t)n;
    return (sn * 8 - 3) / 5;
}

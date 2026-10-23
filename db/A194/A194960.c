/*
 * A194960  a(n) = floor((n+2)/3) + ((n-1) mod 3).
 * https://oeis.org/A194960
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n+2)/3) + ((n-1) mod 3)
 * Range:     u8 n <= 761, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the two components using idx_t to avoid overflow when n is large */
    idx_t part1 = (n + 2) / 3;   /* floor((n+2)/3) */
    idx_t part2 = (n - 1) % 3;   /* (n-1) mod 3 */
    return (seq_t)(part1 + part2);
}

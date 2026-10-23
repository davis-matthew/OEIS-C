/*
 * A171182  Period 6: repeat [0, 1, 1, 1, 0, 2].
 * https://oeis.org/A171182
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = floor(((n+1) mod 6)/3) + 2*floor(((n+5) mod 6)/5)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t t1 = (n + 1) % 6;
    idx_t t2 = (n + 5) % 6;
    seq_t part1 = (seq_t)(t1 / 3);
    seq_t part2 = 2 * (seq_t)(t2 / 5);
    return part1 + part2;
}

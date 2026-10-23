/*
 * A108248  a(n) = ceiling(n/24) + ceiling((n+1)/24).
 * https://oeis.org/A108248
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil(n/24) + ceil((n+1)/24)
 * Range:     u8 n <= 101, u16 n <= 101, u32 n <= 101, u64 n <= 101,
 *            u128 n <= 101, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    unsigned long long nn = (unsigned long long)n;
    unsigned long long part1 = (nn + 23ULL) / 24ULL;
    unsigned long long part2 = (nn + 24ULL) / 24ULL;
    unsigned long long sum  = part1 + part2;
    return (seq_t)sum;
}

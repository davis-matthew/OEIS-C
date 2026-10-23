/*
 * A130485  a(n) = Sum_{k=0..n} (k mod 7) (Partial sums of A010876).
 * https://oeis.org/A130485
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 21*floor(n/7) + (n mod 7)*(n mod 7 + 1)/2
 * Range:     u8 n <= 86, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t q = n / 7ULL;          /* floor(n/7) */
    idx_t r = n % 7ULL;          /* n mod 7 */

    seq_t part1 = (seq_t)21 * (seq_t)q;
    seq_t part2 = (seq_t)r * (seq_t)(r + 1) / 2;

    return part1 + part2;
}

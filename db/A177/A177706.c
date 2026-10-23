/*
 * A177706  Period 5: repeat [1, 1, 1, 1, 2].
 * https://oeis.org/A177706
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(n) = 1 if n % 5 != 4, otherwise 2. Pattern repeats [1,1,1,1,2]
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (n % 5ULL == 4ULL) ? 2 : 1;
}

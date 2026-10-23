/*
 * A131806  Period 4: repeat [0, 2, 4, 6].
 * https://oeis.org/A131806
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 * ((n-1) mod 4)
 * Range:     u8 n <= 108, u16 n <= 108, u32 n <= 108, u64 n <= 108,
 *            u128 n <= 108, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* below offset, value irrelevant */
    return (seq_t)(((n - 1) & 3ULL) * 2ULL);
}

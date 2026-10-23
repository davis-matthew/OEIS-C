/*
 * A168098  a(n) = number of natural numbers m such that n - 8 <= m <= n + 8.
 * https://oeis.org/A168098
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = 8 + n for 0 ≤ n ≤ 8, a(n) = 17 for n ≥ 9
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n <= 8)
        return (seq_t)n + (seq_t)8;
    else
        return (seq_t)17;
}

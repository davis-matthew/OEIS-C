/*
 * A222260  Lexicographically earliest injective sequence of nonnegative
 *          integers such that the sum of 10 consecutive terms is always
 *          divisible by 10.
 * https://oeis.org/A222260
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n unless n = 4 (mod 5) in which case a(n) = 2n-4
 * Range:     u8 n <= 133, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t val = (seq_t)n;
    return (n % 5 == 4) ? (val * 2 - 4) : val;
}

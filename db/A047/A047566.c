/*
 * A047566  Numbers that are congruent to {4, 5, 6, 7} mod 8.
 * https://oeis.org/A047566
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*floor((n-1)/4) + ((n-1)%4) + 4, for n>=1
 * Range:     u8 n <= 128, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t t = (seq_t)n - 1;
    seq_t block = t / 4;
    seq_t rem = t % 4;
    return 8 * block + 4 + rem;
}

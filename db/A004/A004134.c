/*
 * A004134  Denominators in expansion of (1-x)^{-1/4} are 2^a(n).
 * https://oeis.org/A004134
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*n - A000120(n), where A000120(n) is the popcount of n
 * Range:     u8 n <= 86, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    return 3 * (seq_t)n - (seq_t)__builtin_popcountll((unsigned long long)n);
}

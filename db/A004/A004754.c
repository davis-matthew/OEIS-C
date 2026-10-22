/*
 * A004754  Numbers whose binary expansion starts 10.
 * https://oeis.org/A004754
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = n + 2^floor(log_2(n))
 * Range:     u8 n <= 127, u16 n <= 1023, u32 n <= 1023, u64 n <= 1023,
 *            u128 n <= 1023, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t log2n = 63 - __builtin_clzll(n);
    return (seq_t)n + ((seq_t)1 << log2n);
}

/*
 * A157738  a(n) = 18522n - 42.
 * https://oeis.org/A157738
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 18522n - 42
 * Range:     u8 none, u16 n <= 3, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    return (seq_t)n * (seq_t)18522 - (seq_t)42;
}

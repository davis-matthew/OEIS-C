/*
 * A130518  a(n) = Sum_{k=0..n} floor(k/3). (Partial sums of A002264.)
 * https://oeis.org/A130518
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*(n-1)/6)
 * Range:     u8 n <= 39, u16 n <= 627, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Compute n*(n-1) in seq_t first, then divide by 6. */
    return ((seq_t)n * ((seq_t)n - 1u)) / 6;
}

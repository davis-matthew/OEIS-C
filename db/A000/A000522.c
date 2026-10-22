/*
 * A000522  Total number of ordered k-tuples (k=0..n) of distinct elements
 *          from an n-element set: a(n) = Sum_{k=0..n} n!/k!.
 * https://oeis.org/A000522
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice, changed
 * Formula:   a(n) = n*a(n-1) + 1, a(0) = 1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 1;
    for (idx_t i = 1; i <= n; i++) {
        result = (seq_t)i * result + 1;
    }
    return result;
}

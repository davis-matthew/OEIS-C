/*
 * A120573  a(n) = n^5 + 3*n^3 + 2*n = n*(n^2 + 1)*(n^2 + 2).
 * https://oeis.org/A120573
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n^2+1)*(n^2+2)
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 84, u64 n <= 7131,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Convert the index to the full integer type before any arithmetic. */
    seq_t nn = (seq_t)n;
    /* Compute n^2 once. */
    seq_t sq = nn * nn;
    /* Compute the product n*(n^2+1)*(n^2+2) in a safe order. */
    seq_t part1 = sq + (seq_t)1;      /* n^2 + 1 */
    seq_t part2 = sq + (seq_t)2;      /* n^2 + 2 */
    seq_t res   = nn * part1 * part2; /* n * (n^2+1) * (n^2+2) */
    return res;
}

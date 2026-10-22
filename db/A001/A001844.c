/*
 * A001844  Centered square numbers: a(n) = 2*n*(n+1)+1. Sums of two
 *          consecutive squares. Also, consider all Pythagorean triples (X, Y,
 *          Z=Y+1) ordered by increasing Z; then sequence gives Z values.
 * https://oeis.org/A001844
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 2*n*(n+1) + 1
 * Range:     u8 n <= 10, u16 n <= 180, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;
    return 2 * n_seq * (n_seq + 1) + 1;
}

/*
 * A001316  Gould's sequence: a(n) = Sum_{k=0..n} (binomial(n,k) mod 2);
 *          number of odd entries in row n of Pascal's triangle (A007318);
 *          a(n) = 2^A000120(n).
 * https://oeis.org/A001316
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 2^A000120(n), where A000120(n) is the number of 1-bits in
 *            n
 * Range:     u8 n <= 254, u16 n <= 9217, u32 n <= 9217, u64 n <= 9217,
 *            u128 n <= 9217, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    return (seq_t)1 << __builtin_popcountll(n);
}

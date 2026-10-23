/*
 * A047606  Numbers that are congruent to {1, 2, 3, 5} mod 8.
 * https://oeis.org/A047606
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*((n-1)/4) + [1,2,3,5][(n-1) mod 4]
 * Range:     u8 n <= 128, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 1, a(2) = 2, a(3) = 3, a(4) = 5, etc. */
seq_t a(idx_t n) {
    /* mapping for residues within each block of 8 */
    static const seq_t m[4] = {(seq_t)1, (seq_t)2, (seq_t)3, (seq_t)5};
    idx_t block = (n - 1) / 4;
    idx_t idx   = (n - 1) % 4;
    return (seq_t)block * (seq_t)8 + m[idx];
}

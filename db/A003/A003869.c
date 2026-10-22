/*
 * A003869  Degrees of irreducible representations of symmetric group S_5.
 * https://oeis.org/A003869
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   degrees of irreducible representations of S_5, sorted
 * Range:     u8 n <= 7, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static const seq_t degrees[] = {1, 1, 4, 4, 5, 5, 6};
    if (n < 1 || n > 7) return 0;
    return degrees[(int)(n - 1)];
}

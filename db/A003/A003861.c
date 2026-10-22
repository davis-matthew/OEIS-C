/*
 * A003861  Degrees of irreducible representations of alternating group A_6.
 * https://oeis.org/A003861
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Degrees of irreducible representations of alternating group A_6 */
seq_t a(idx_t n) {
    static const seq_t degrees[] = {1, 5, 5, 8, 8, 9, 10, 10, 16, 16};
    if (n >= 1 && n <= 10) {
        return degrees[n - 1];
    }
    return 0;
}

/*
 * A003860  Degrees of irreducible representations of alternating group A_5.
 * https://oeis.org/A003860
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Finite sequence of degrees of irreducible representations of A_5 */
seq_t a(idx_t n) {
    static const seq_t values[] = {0, 1, 3, 3, 4, 5};
    return (n <= 5) ? values[n] : 0;
}

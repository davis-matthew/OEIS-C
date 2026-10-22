/*
 * A003879  Degrees of irreducible representations of group L2(7).
 * https://oeis.org/A003879
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   Degrees of irreducible representations of group L2(7)
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static const seq_t degrees[] = {1, 3, 3, 6, 7, 8};
    
    if (n < 1 || n > 6) {
        return 0;
    }
    
    return degrees[n - 1];
}

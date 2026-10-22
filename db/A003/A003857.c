/*
 * A003857  Degrees of irreducible representations of Mathieu group M_22.
 * https://oeis.org/A003857
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Degrees of irreducible representations of Mathieu group M_22; finite sequence with 12 terms */

seq_t a(idx_t n) {
    static const seq_t degrees[] = {
        1, 21, 45, 45, 55, 99, 154, 210, 231, 280, 280, 385
    };
    
    if (n >= 1 && n <= 12) {
        return degrees[n - 1];
    }
    return 0;
}

/*
 * A003880  Degrees of irreducible representations of group L2(8).
 * https://oeis.org/A003880
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Degrees of irreducible representations of group L2(8). Finite sequence with 9 terms. */

seq_t a(idx_t n)
{
    static const seq_t values[] = {0, 1, 7, 7, 7, 7, 8, 9, 9, 9};
    
    if (n <= 9) {
        return values[n];
    }
    return 0;
}

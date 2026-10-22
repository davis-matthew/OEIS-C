/*
 * A005455  A finite sequence associated with the Lie algebra C_4.
 * https://oeis.org/A005455
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Finite sequence: a(n) = values[n] for n=0..12, 0 for n>=13 */
seq_t a(idx_t n) {
    if (n >= 13) return 0;
    const seq_t values[] = {1, 2, 3, 4, 8, 9, 10, 11, 12, 20, 35, 36, 84};
    return values[n];
}

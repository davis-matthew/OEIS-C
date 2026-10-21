/*
 * A080992  Entries in Dürer's magic square.
 * https://oeis.org/A080992
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   a(n) = Dürer's magic square entry at position (n-1) mod 16
 *            (row-major order)
 * Range:     u8 n <= 16, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: 16, 3, 2, 13, 5, 10, 11, 8, 9, 6, 7, 12, 4, 15, 14, 1 repeats with period 16. */
seq_t a(idx_t n)
{
    /* 4x4 magic square from Dürer's Melancolia (row-major). */
    static const seq_t sq[16] = {
        16, 3, 2, 13,
        5, 10, 11, 8,
        9, 6, 7, 12,
        4, 15, 14, 1
    };
    if (n == 0)          /* safety: sequence is defined for n >= 1 */
        return 0;
    idx_t pos = (n - 1) % 16;
    return sq[pos];
}

/*
 * A030241  Minimal determinant of any n-dimensional even lattice.
 * https://oeis.org/A030241
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sequence with period 8: 1,2,3,4,4,4,3,2 repeating
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* table holds the periodic pattern */
    static const seq_t tab[8] = { (seq_t)1, (seq_t)2, (seq_t)3, (seq_t)4,
                                  (seq_t)4, (seq_t)4, (seq_t)3, (seq_t)2 };
    /* use bitmask instead of modulo for speed (8 is 2^3) */
    return tab[n & 7];
}

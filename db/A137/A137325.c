/*
 * A137325  Number of terms in the Janet periodic table of the elements 32
 *          columns: ordered 14 2's, 10 4's, 6 6's, 2 8's.
 * https://oeis.org/A137325
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = 2 if (n-1) mod 32 < 14; 4 if (n-1) mod 32 < 24; 6 if
 *            (n-1) mod 32 < 30; otherwise 8
 * Range:     u8 n <= 32, u16 n <= 32, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t j = (n - 1) % 32;
    if (j < 14) return (seq_t)2;
    if (j < 24) return (seq_t)4;
    if (j < 30) return (seq_t)6;
    return (seq_t)8;
}

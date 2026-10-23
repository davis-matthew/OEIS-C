/*
 * A131735  Period 6: repeat [0, 0, 1, 1, 1, 1].
 * https://oeis.org/A131735
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if n mod 6 >= 2 else 0
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute n modulo 6. */
    idx_t r = n % 6;
    /* Return 1 for residues 2–5, else 0. */
    return (r >= 2) ? (seq_t)1 : (seq_t)0;
}

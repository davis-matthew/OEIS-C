/*
 * A010384  Squares mod 22.
 * https://oeis.org/A010384
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = periodic sequence of quadratic residues modulo 22: 0, 1,
 *            3, 4, 5, 9, 11, 12, 14, 15, 16, 20 repeating (period 12)
 * Range:     u8 n <= 12, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The 12 residues of squares mod 22 in sorted order. */
    static const seq_t residues[12] = {0, 1, 3, 4, 5, 9, 11, 12, 14, 15, 16, 20};

    /* Map n (1‑based) to its index in the periodic array (0‑based). */
    idx_t idx = (n - 1) % 12;

    return residues[idx];
}

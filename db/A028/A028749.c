/*
 * A028749  Nonsquares mod 36.
 * https://oeis.org/A028749
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = the n‑th positive integer x with * x mod 36 not in
 *            {0,1,4,9,13,16,25,28}. That is, a(n) * enumerates all nonsquare
 *            residues modulo 36
 * Range:     u8 n <= 28, u16 n <= 28, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t residues[28] = {
        2, 3, 5, 6, 7, 8, 10, 11, 12, 14, 15, 17,
        18, 19, 20, 21, 22, 23, 24, 26, 27, 29, 30, 31,
        32, 33, 34, 35
    };
    idx_t block = (n - 1) / 28;   /* number of full cycles of 28 residues */
    idx_t pos   = (n - 1) % 28;   /* position within the current cycle */
    return (seq_t)block * 36u + residues[pos];
}

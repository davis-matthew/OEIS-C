/*
 * A010389  Squares mod 27.
 * https://oeis.org/A010389
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = 27 * floor((n-1)/11) + residues[(n-1)%11], where residues
 *            = {0,1,4,7,9,10,13,16,19,22,25}
 * Range:     u8 n <= 11, u16 n <= 11, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t residues[11] = {0, 1, 4, 7, 9, 10, 13, 16, 19, 22, 25};
    idx_t zero = n - 1;          // n >= 1, unsigned
    idx_t block = zero / 11;     // number of full 27-blocks
    idx_t idx = zero % 11;       // index into residue set
    return (seq_t)block * (seq_t)27 + residues[idx];
}

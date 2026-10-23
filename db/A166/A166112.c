/*
 * A166112  Numbers n with property that n + 2^n == 0 (mod 17).
 * https://oeis.org/A166112
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = residues[(n-1)%8] + 136*((n-1)/8)
 * Range:     u8 n <= 15, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Residues of solutions of n + 2^n ≡ 0 (mod 17) modulo 136 */
seq_t a(idx_t n)
{
    static const seq_t residues[8] = {16, 38, 43, 49, 52, 53, 98, 127};
    idx_t idx = n - 1;           /* safe : n >= 1 */
    idx_t q   = idx / 8;          /* number of complete blocks of 136 */
    idx_t r   = idx % 8;          /* position within the current block */
    return ((seq_t)residues[r]) + ((seq_t)136 * (seq_t)q);
}

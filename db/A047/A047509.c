/*
 * A047509  Numbers that are congruent to {0, 1, 4, 6, 7} mod 8.
 * https://oeis.org/A047509
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*floor((n-1)/5) + [0,1,4,6,7][(n-1)%5]
 * Range:     u8 n <= 160, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* assume n >= 1, behaviour for n < 1 is not defined */
    /* offset n to 0‑based index */
    idx_t n0 = n - 1;
    idx_t block = n0 / 5;
    idx_t offset = n0 % 5;
    /* residues that satisfy the congruence condition modulo 8 */
    static const seq_t RES[5] = {0, 1, 4, 6, 7};
    return (seq_t)block * (seq_t)8 + RES[offset];
}

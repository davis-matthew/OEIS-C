/*
 * A047567  Numbers that are congruent to {0, 4, 5, 6, 7} mod 8.
 * https://oeis.org/A047567
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(5k+1)=8k, a(5k+2)=8k+4, a(5k+3)=8k+5, a(5k+4)=8k+6,
 *            a(5k+5)=8k+7
 * Range:     u8 n <= 160, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)           /* unused: offset starts at 1 */
        return 0;
    /* Compute q = floor((n-1)/5) and r = (n-1) mod 5 */
    idx_t n1 = n - 1;
    idx_t q  = n1 / 5;
    idx_t r  = n1 % 5;
    /* Offsets of the allowed residues mod 8 */
    static const seq_t offset[5] = {0, 4, 5, 6, 7};
    return (seq_t)q * 8 + offset[r];
}

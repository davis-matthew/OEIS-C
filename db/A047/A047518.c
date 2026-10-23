/*
 * A047518  Numbers that are congruent to {2, 3, 4, 6, 7} mod 8.
 * https://oeis.org/A047518
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*floor((n-1)/5) + {2,3,4,6,7}[ (n-1) mod 5 ]
 * Range:     u8 n <= 160, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Return 0 for n == 0 (out of offset); behaviour below offset is undefined */
    if (n == 0)
        return (seq_t)0;

    /* Compute quotient and remainder of (n-1) divided by 5 */
    idx_t m = n - 1;
    idx_t q = m / 5;              /* integer division */
    int r = (int)(m % 5);         /* remainder in 0..4 */

    /* Offsets for the five allowed residues modulo 8 */
    static const seq_t offset[5] = { (seq_t)2, (seq_t)3, (seq_t)4,
                                     (seq_t)6, (seq_t)7 };

    /* Base is 8*q; add the appropriate offset */
    seq_t base = (seq_t)8 * (seq_t)q;
    return base + offset[r];
}

/*
 * A047517  Numbers that are congruent to {0, 1, 3, 4, 6, 7} mod 8.
 * https://oeis.org/A047517
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*floor((n-1)/6) + [0,1,3,4,6,7][ (n-1)%6 ]
 * Range:     u8 n <= 192, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(1) is the first term, offset = 1. */
    if (n == 0)
        return 0;                 /* undefined for n < 1, but harmless */

    /* Compute block number and offset within the block. */
    idx_t q = (n - 1) / 6;         /* floor((n-1)/6) */
    idx_t r = (n - 1) % 6;         /* (n-1) mod 6 */

    /* The residues that are included in each block of 8 integers. */
    static const seq_t block[6] = {
        (seq_t)0, (seq_t)1, (seq_t)3,
        (seq_t)4, (seq_t)6, (seq_t)7
    };

    /* Return the nth term as the value of the current block plus the offset. */
    return ((seq_t)q * (seq_t)8) + block[r];
}

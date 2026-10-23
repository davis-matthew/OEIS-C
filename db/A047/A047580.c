/*
 * A047580  Numbers that are congruent to {1, 2, 5, 6, 7} mod 8.
 * https://oeis.org/A047580
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8 * floor((n-1)/5) + {1,2,5,6,7}[(n-1)%5]
 * Range:     u8 n <= 160, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for offset 0, guarded */

    /* residues allowed in order within each block of eight */
    const seq_t offs[5] = {1, 2, 5, 6, 7};

    /* (n-1) because the sequence starts at n = 1 */
    idx_t q = (n - 1) / 5;         /* block index */
    idx_t r = (n - 1) % 5;         /* position within the block */

    seq_t base = (seq_t)q * (seq_t)8;   /* 8 * q  */
    return base + offs[r];           /* a(n) */
}

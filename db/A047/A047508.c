/*
 * A047508  Numbers that are congruent to {1, 4, 6, 7} mod 8.
 * https://oeis.org/A047508
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8 * ceil((n-1)/4) + {1,4,6,7}[(n-1) mod 4]
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*         a(1) = 1, a(2) = 4, a(3) = 6, a(4) = 7, a(5) = 9, ... */

seq_t a(idx_t n)
{
    /* offset values for the four allowed residues mod 8 */
    static const idx_t off[4] = {1, 4, 6, 7};

    /* (n-1) since the sequence starts at 1 */
    idx_t m = n - 1;
    idx_t k = m / 4;      /* number of complete groups of 4 */
    idx_t r = m % 4;      /* position within the group */

    idx_t val = 8 * k + off[r];
    return (seq_t) val;
}

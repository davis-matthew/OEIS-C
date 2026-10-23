/*
 * A047513  Numbers that are congruent to {0, 1, 2, 4, 6, 7} mod 8.
 * https://oeis.org/A047513
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*floor((n-1)/6) + {0,1,2,4,6,7}[ (n-1)%6 ]
 * Range:     u8 n <= 192, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* mapping of the six accepted residues in each block of 8 consecutive integers */
    static const seq_t offs[6] = {0, 1, 2, 4, 6, 7};

    /* The sequence is defined only for n >= 1 (a(1) = 0).  For n == 0 we simply return 0. */
    if (n == 0) return 0;

    idx_t q = (n - 1) / 6;          /* number of completed blocks of 6 values */
    idx_t r = (n - 1) % 6;          /* position inside the current block */

    seq_t base = (seq_t)q * 8;      /* base value of the current block */
    return base + offs[r];          /* add the appropriate residue offset */
}

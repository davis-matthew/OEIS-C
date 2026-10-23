/*
 * A047605  Numbers that are congruent to {0, 2, 3, 5} mod 8.
 * https://oeis.org/A047605
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8 * ((n-1)/4) + {0, 2, 3, 5}[(n-1)%4]
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the block number and position within the block */
    idx_t k = (n - 1) / 4;    /* number of complete 8‑number blocks before a(n) */
    idx_t r = (n - 1) % 4;    /* position within the current block */

    /* Offsets of the four accepted residues modulo 8 */
    static const seq_t offs[4] = {0, 2, 3, 5};

    return (seq_t)8 * (seq_t)k + offs[r];
}

/*
 * A047571  Numbers that are congruent to {0, 2, 4, 5, 6, 7} mod 8.
 * https://oeis.org/A047571
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*floor((n-1)/6) + {0,2,4,5,6,7}[((n-1)%6)]
 * Range:     u8 n <= 192, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* offset 1: n starts at 1 */
    idx_t k = (n - 1) / 6;          /* number of complete 8‑blocks before n */
    idx_t r = (n - 1) % 6;          /* position inside the current block */
    static const seq_t residues[6] = {0, 2, 4, 5, 6, 7};
    return (seq_t)(8 * k) + residues[r];
}

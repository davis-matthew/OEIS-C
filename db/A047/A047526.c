/*
 * A047526  Numbers that are congruent to {1, 2, 7} mod 8.
 * https://oeis.org/A047526
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*floor((n-1)/3) + {1,2,7}[ (n-1)%3 ]
 * Range:     u8 n <= 96, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* undefined for n<1, but harmless */
    idx_t q = (n - 1) / 3;        /* number of complete 8‑blocks */
    idx_t r = (n - 1) % 3;        /* position inside the block (0,1,2) */
    static const seq_t off[3] = {1, 2, 7};
    return (seq_t)8 * (seq_t)q + off[r];
}

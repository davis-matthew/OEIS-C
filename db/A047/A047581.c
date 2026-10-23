/*
 * A047581  Numbers that are congruent to {0, 1, 2, 5, 6, 7} mod 8.
 * https://oeis.org/A047581
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8 * floor((n-1)/6) + {0,1,2,5,6,7}[(n-1) mod 6]
 * Range:     u8 n <= 192, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t m = n - 1;          /* zero‑based index of the term (a(1) → m=0) */
    idx_t g = m / 6;          /* number of complete 8‑block groups before the term */
    idx_t r = m % 6;          /* position within the 6‑term group */
    static const seq_t offsets[6] = {0, 1, 2, 5, 6, 7};
    return (seq_t)g * ((seq_t)8) + offsets[r];
}

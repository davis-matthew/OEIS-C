/*
 * A082977  Numbers that are congruent to {0, 1, 3, 5, 6, 8, 10} mod 12.
 * https://oeis.org/A082977
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(12*(n-1)/7) (OEIS entry A082977 – numbers ≡
 *            {0,1,3,5,6,8,10} (mod 12)
 * Range:     u8 n <= 150, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1) return 0;                /* a(1)=0, values below offset are irrelevant */
    /* compute 12*(n-1) first in 64‑bit arithmetic to avoid overflow on small seq_t types */
    idx_t prod = 12 * ((idx_t)n - 1);
    return (seq_t)(prod / 7);
}

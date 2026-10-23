/*
 * A047344  Numbers that are congruent to {0, 1, 3, 4} mod 7.
 * https://oeis.org/A047344
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7 * floor((n-1)/4) + [0,1,3,4][(n-1) mod 4]
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers that are congruent to {0,1,3,4} (mod 7) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n<1, return 0 */
    idx_t k = n - 1;
    seq_t block = ((seq_t)k) / 4;   /* number of full 7‑blocks before a(n) */
    unsigned r = (unsigned)(k % 4); /* position within the current block */
    static const seq_t offs[4] = {0, 1, 3, 4};
    return 7 * block + offs[r];
}

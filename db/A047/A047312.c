/*
 * A047312  Numbers that are congruent to {0, 4, 5, 6} mod 7.
 * https://oeis.org/A047312
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*floor((n-1)/4) + {0,4,5,6}[ (n-1)%4 ]
 * Range:     u8 n <= 145, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* sequence of numbers congruent to {0,4,5,6} modulo 7 (offset 1) */
seq_t a(idx_t n)
{
    static const seq_t OFFSETS[4] = {0, 4, 5, 6};
    idx_t q = (n - 1) / 4;          /* block index */
    idx_t r = (n - 1) % 4;          /* position within block */
    return ((seq_t)q) * 7 + OFFSETS[r];
}

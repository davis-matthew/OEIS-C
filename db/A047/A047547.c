/*
 * A047547  Numbers that are congruent to {0, 2, 3, 4, 7} mod 8.
 * https://oeis.org/A047547
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n‑5)+8 for n>5; a(1)=0, a(2)=2, a(3)=3, a(4)=4, a(5)=7
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const seq_t _off[5] = {0, 2, 3, 4, 7};

seq_t a(idx_t n)
{
    idx_t q = (n - 1) / 5;      /* number of full 5‑term blocks before n */
    idx_t r = (n - 1) % 5;      /* position within the block (0…4) */
    return (seq_t)q * 8 + _off[r];
}

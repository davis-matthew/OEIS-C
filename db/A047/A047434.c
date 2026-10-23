/*
 * A047434  Numbers that are congruent to {0, 2, 4, 5, 6} mod 8.
 * https://oeis.org/A047434
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*floor((n-1)/5) + {0,2,4,5,6}[ (n-1)%5 ] (n >= 1)
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)       /* undefined for n < 1, return 0 for safety */
        return (seq_t)0;

    idx_t q = (n - 1) / 5;   /* number of complete 8‑length blocks */
    idx_t r = (n - 1) % 5;   /* position within the block (0‑4) */

    static const unsigned char map[5] = {0, 2, 4, 5, 6};

    return (seq_t)8 * (seq_t)q + (seq_t)map[r];
}

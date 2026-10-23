/*
 * A047575  Numbers that are congruent to {0, 5, 6, 7} mod 8.
 * https://oeis.org/A047575
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*floor((n-1)/4) + {0,5,6,7}[(n-1)%4] (n ≥ 1)
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)   /* safety guard – the sequence starts at n = 1 */
        return (seq_t)0;

    idx_t block = (n - 1) / 4;          /* number of complete 4‑term blocks before n */
    idx_t pos   = (n - 1) % 4;          /* position within the current block (0…3) */

    /* The four numbers in each 8‑number block, in order */
    static const seq_t offsets[4] = {
        (seq_t)0,
        (seq_t)5,
        (seq_t)6,
        (seq_t)7
    };

    /* Combine the block offset and the intra‑block offset */
    return block * (seq_t)8 + offsets[(unsigned int)pos];
}

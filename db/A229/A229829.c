/*
 * A229829  Numbers coprime to 15.
 * https://oeis.org/A229829
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 15*floor((n-1)/8) + [1,2,4,7,8,11,13,14][(n-1)%8]
 * Range:     u8 n <= 136, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* offset is 1, 0 is unused */
    static const seq_t pattern[8] = {
        (seq_t)1, (seq_t)2, (seq_t)4, (seq_t)7,
        (seq_t)8, (seq_t)11, (seq_t)13, (seq_t)14
    };
    seq_t k = (seq_t)((n - 1) / 8);      /* block number */
    unsigned r = (unsigned)((n - 1) % 8);/* offset within the block */
    return ((seq_t)15) * k + pattern[r];
}

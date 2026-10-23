/*
 * A047236  Numbers that are congruent to {1, 2, 4} mod 6.
 * https://oeis.org/A047236
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*floor((n-1)/3) + [1,2,4][(n-1)%3]
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the block index k and the position r inside the block. */
    seq_t k = (((seq_t)n) - (seq_t)1) / (seq_t)3;
    idx_t r = (((idx_t)n) - (idx_t)1) % (idx_t)3;

    /* Offsets for the residues 1, 2 and 4 modulo 6. */
    static const seq_t offsets[3] = { (seq_t)1, (seq_t)2, (seq_t)4 };

    return k * (seq_t)6 + offsets[(int)r];
}

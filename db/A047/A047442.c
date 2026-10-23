/*
 * A047442  Numbers that are congruent to {0, 1, 2, 5, 6} mod 8.
 * https://oeis.org/A047442
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*floor((n-1)/5) + offset[(n-1) % 5], where offset =
 *            {0,1,2,5,6}
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert to 0‑based index */
    idx_t k = n - 1;
    /* Compute large block index (5 numbers per block) */
    idx_t q = k / 5;
    /* Remainder within the block */
    idx_t r = k % 5;

    /* Offsets of the valid residues modulo 8 */
    static const seq_t OFF[5] = {0, 1, 2, 5, 6};

    /* 8*q + offset[r] yields the nth term */
    return (seq_t)8 * (seq_t)q + OFF[r];
}

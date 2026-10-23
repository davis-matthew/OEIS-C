/*
 * A047397  Numbers that are congruent to {0, 1, 2, 6} mod 8.
 * https://oeis.org/A047397
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*⌊(n-1)/4⌋ + offset[(n-1) mod 4] , offset = {0,1,2,6}
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is 1‑based */
    idx_t idx   = n - 1;          /* 0‑based index of the desired term */
    idx_t block = idx / 4;        /* which block of size 8 we are in   */
    idx_t pos   = idx % 4;        /* position inside the block          */

    /* offsets for positions 0..3 within a block of 8
       values that are ≡ 0,1,2,6 (mod 8)           */
    static const seq_t offsets[4] = {0, 1, 2, 6};

    seq_t base = (seq_t)block * (seq_t)8;   /* 8 * block, cast to seq_t */
    return base + offsets[pos];
}

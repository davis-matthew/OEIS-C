/*
 * A047502  Numbers that are congruent to {2, 3, 4, 5, 7} mod 8.
 * https://oeis.org/A047502
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(5k) = 8k-1 a(5k-1) = 8k-3 a(5k-2) = 8k-4 a(5k-3) = 8k-5
 *            a(5k-4) = 8k-6 (valid for k >= 1)
 * Range:     u8 n <= 160, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of A047502 (numbers congruent to 2,3,4,5,7 mod 8) */
seq_t a(idx_t n)
{
    /* Since the sequence repeats every 5 indices, compute the block number
       and the offset within the block.  Blocks are 1‑based, named by k.  */
    idx_t block = (n - 1) / 5;         /* 0‑based block index (k-1)   */
    seq_t base  = (seq_t)(block + 1) * (seq_t)8;  /* 8*k */

    /* Subtractions for offsets 0..4  (corresponding to residues 2,3,4,5,7) */
    static const seq_t sub[5] = { 6, 5, 4, 3, 1 };

    idx_t offset = (n - 1) % 5;        /* 0..4 */

    return base - sub[offset];
}

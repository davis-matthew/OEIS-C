/*
 * A088227  Solutions x to x^n == 7 mod 13.
 * https://oeis.org/A088227
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 13*floor((n-1)/4) + [2,6,7,11][(n-1)%4]
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This sequence consists of all positive integers congruent to 2,6,7 or 11 (mod 13). */
seq_t a(idx_t n)
{
    /* Decompose n–1 into block (size 4) and offset within the block */
    const seq_t offs[4] = {2, 6, 7, 11};
    idx_t  block_idx = (n - 1) / 4;          /* 0‑based block number */
    unsigned offset   = (unsigned)((n - 1) & 3); /* residue index 0‑3 */

    /* Use seq_t arithmetic to avoid overflow on intermediate results. */
    seq_t block  = (seq_t)block_idx;         /* safe cast: block_idx < n */
    return block * 13 + offs[offset];
}

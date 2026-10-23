/*
 * A194063  Natural fractal sequence of A006578.
 * https://oeis.org/A194063
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is defined by successive blocks whose lengths are 3,4,6,7,9,10,...
   The block lengths satisfy:  L0 = 3,
                                L_{i+1} = L_i + 1 + (i % 2)
   where i is the zero‑based block index.  The value a(n) is the
   offset (starting at 1) of n within its block.  That is,
   if B_k is the block containing n and C_k = Σ_{i<k} L_i is the
   cumulative length of preceding blocks, then
     a(n) = n - C_k .
*/

seq_t a(idx_t n)
{
    idx_t cum = 0;          /* sum of lengths of previous blocks */
    idx_t len = 3;          /* length of current block */
    idx_t i   = 0;          /* zero‑based index of current block */

    /* Move to the block that contains n */
    while (cum + len < n) {
        cum += len;            /* accumulate previous block */
        len += 1 + (i % 2);    /* compute next block length */
        ++i;                   /* advance to next block index */
    }

    return (seq_t)(n - cum);   /* offset within the current block (1‑based) */
}

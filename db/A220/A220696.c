/*
 * A220696  The positions of those permutations in A030298 where the first
 *          element is one (fixed).
 * https://oeis.org/A220696
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 136, u16 n <= 5914, u32 n <= 5914, u64 n <= 5914,
 *            u128 n <= 5914, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Positions of the permutations in A030298 whose first element is 1.
   For n>=1 the term a(n) equals the index (1‑based) of the
   nth such permutation in the lexicographically ordered list of all
   permutations of the natural numbers.
   Let N be the smallest integer with
         sum_{k=1}^{N-1} (k-1)! + (N-1)!  ≥ n
   Then
         a(n) = (∑_{k=1}^{N-1} k!) + 1 + n - (∑_{k=1}^{N-1} (k-1)! ) - 1
            = start_of_block_N + offset_within_block. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined for offset 0 – return 0 */

    seq_t cumulative   = 0;              /* sum_{k=1}^{N-1} (k-1)!  */
    seq_t sum_factorial = 0;             /* sum_{k=1}^{N-1}   k!    */
    idx_t  N = 1;                        /* current block index   */
    seq_t  block_size = 1;               /* (N-1)!  for the current block */

    /* Find the block that contains the nth term. */
    while (cumulative + block_size < n) {
        cumulative += block_size;        /* finished current block   */
        N++;                            /* move to next block      */
        block_size *= (seq_t)(N - 1);    /* new block_size = (N-1)! */
        sum_factorial += block_size;    /* accumulate factorials   */
    }

    seq_t start_of_block = sum_factorial + 1;         /* first index of this block */
    seq_t offset          = (seq_t)n - cumulative - 1;/* 0‑based offset inside block */

    return start_of_block + offset;
}

/*
 * A171178  Permutation of the natural numbers: 0 together with the partial
 *          sums of A171177.
 * https://oeis.org/A171178
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(0)=0.
 * For n≥1 let K be the smallest integer such that K^2+3K+2 ≥ n.
 * Let L = (K-1)^2 + 3K  (the first index of block K, except block 1).
 * Block 1 (n=1..6) is special:
 *     a(1)=2, a(2)=1, a(3)=6, a(4)=3, a(5)=5, a(6)=4.
 * For K≥2 the block size is:
 *     s = 6   if K==2
 *     s = 2·(K+1)   otherwise.
 * Let R = L + s - 1  (last index of the block).
 * Let pos = n – L   (0‑based position inside the block).
 * Then
 *     if (pos is even)  a(n) = R - pos/2;
 *     else              a(n) = L + pos/2;
 *
 * This produces the OEIS sequence A171178.
 */
seq_t a(idx_t n)
{
    /* special case: 0 at index 0 */
    if (n == 0)
        return (seq_t)0;

    /* first block of 6 entries */
    if (n <= 6) {
        static const seq_t first_block[7] = {0, 2, 1, 6, 3, 5, 4};
        return first_block[n];
    }

    /* find smallest K with K^2 + 3K + 2 >= n; K >= 2 here */
    idx_t K = 2;
    /* loop is very small because n is at most a few thousand here; 
       it stays < ~100 for n < 10 000, so a plain while is fine */
    while ((K * K + 3 * K + 2) < n)
        ++K;

    /* start index of block K (for K>=2) */
    idx_t start = (K - 1) * (K - 1) + 3 * K;

    /* block size */
    idx_t block_size = (K == 2) ? 6 : 2 * (K + 1);

    /* end index of the block */
    idx_t R = start + block_size - 1;

    /* position inside the block (0‑based) */
    idx_t pos = n - start;

    /* compute a(n) according to the even/odd rule */
    if ((pos & 1) == 0)
        return (seq_t)(R - (pos >> 1));
    else
        return (seq_t)(start + (pos >> 1));
}

/*
 * A134391  The sequence A_0, A_1, A_2, A_3, ..., where the A_k are defined in
 *          A064990.
 * https://oeis.org/A134391
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th bit of the infinite concatenation of the binary
 *            strings A_k, where A_0 = "0" and A_{k+1} = A_k A_k
 *            complement(A_k). The string A_k has length 3^k. Equivalent to a
 *            generalised Thue–Morse construction
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static seq_t get_bit(idx_t len, idx_t pos)
{
    /* len == 3^k, pos < len */
    if (len == 1) return 0;               /* A_0 = "0" */
    idx_t block = len / 3;                /* length of A_{k-1} */
    if (pos < block)                      /* first copy of A_{k-1} */
        return get_bit(block, pos);
    else if (pos < 2 * block)             /* second copy of A_{k-1} */
        return get_bit(block, pos - block);
    else                                 /* third copy – complement */
    {
        seq_t b = get_bit(block, pos - 2 * block);
        return (seq_t)(1 - b);            /* flip 0↔1 */
    }
}

seq_t a(idx_t n)
{
    idx_t cum = 0;   /* cumulative length of blocks processed so far   */
    idx_t len = 1;   /* length of current block A_k (3^k), k = 0 initially */

    /* Find block that contains position n */
    while (cum + len <= n)
    {
        cum += len;
        len *= 3;
    }

    /* Offset of n inside this block */
    idx_t offset = n - cum;
    return get_bit(len, offset);
}

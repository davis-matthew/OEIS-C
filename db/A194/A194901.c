/*
 * A194901  Inverse permutation of A194900; every positive integer occurs
 *          exactly once.
 * https://oeis.org/A194901
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = inverse of the permutation that, for each block of
 *            consecutive integers of length L, produces the order (L-1 or
 *            L-2), (L-3 or L-4), …, 0 followed by (L-2 or L-1), (L-4 or L-3),
 *            …, 1, i.e. all indices descending by 2 from the largest even (or
 *            odd) down to 0, then all indices descending by 2...
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   triangular_block
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/triangular_block.h"

seq_t a(idx_t n)
{
    /* Identity block for the first three integers */
    if (n <= (idx_t)3)
        return (seq_t)n;

    /* Find the block index j ≥ 1 such that n lies in the j‑th block.
       The j‑th block starts after (j+1)(j+2)/2 elements and has length L=j+2. */
    idx_t j = 1;
    while (triangular_block(j) < n)
        ++j;

    idx_t previous_cum = triangular_block(j - 1);   /* elements before this block */
    idx_t start = previous_cum + 1;                 /* first integer of the block */
    idx_t L = j + 2;                                /* block length */

    idx_t offset = n - start;       /* 0‑based position of n within the block */

    /* Build the permutation that yields the indices of the block.
       For offset < A_len use the high‑end sequence, otherwise the low‑end sequence. */
    bool odd = (L & 1);
    idx_t start_high = odd ? (L - 1) : (L - 2);      /* first value in the high list */
    idx_t start_low  = odd ? (L - 2) : (L - 1);      /* first value in the low list */
    idx_t A_len = (L + 1) / 2;                      /* number of elements in the high list */

    idx_t perm_pos;                                 /* position offset where n appears */
    if (offset < A_len) {
        perm_pos = start_high - 2 * offset;
    } else {
        idx_t k = offset - A_len;
        perm_pos = start_low - 2 * k;
    }

    return (seq_t)start + (seq_t)perm_pos;
}

/*
 * A054430  Simple self-inverse permutation of natural numbers: List each
 *          clump of phi(n) numbers (starting from phi(2) = 1) in reverse
 *          order.
 * https://oeis.org/A054430
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) reverses consecutive clumps of natural numbers * whose
 *            lengths are totients φ(k) (k ≥ 2). The first clump has size
 *            φ(2)=1
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The helper `phi` is provided in ../utils/phi.h */
#include "../utils/phi.h"

seq_t a(idx_t n)
{
    /* Find the block that contains position n. */
    idx_t cum = 0;               /* cumulative clip size before current block */
    idx_t blockSize = 0;
    idx_t k = 2;                 /* block index, starting at 2 */

    for (;; ++k)
    {
        blockSize = (idx_t)phi(k);        /* φ(k) fits in idx_t (φ(k) ≤ k) */
        if (cum + blockSize >= n)
            break;
        cum += blockSize;
    }

    /* Position of the first element of this block in the permutation. */
    idx_t startIdx = cum + 1;            /* 1‑based */

    /* Offset of n within this block (0‑based). */
    idx_t offset = n - startIdx;

    /* The element at position n is the element that is offset
     * from the end of the block. */
    idx_t resultIdx = startIdx + blockSize - 1 - offset;

    return (seq_t)resultIdx;
}

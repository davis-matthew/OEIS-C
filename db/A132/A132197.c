/*
 * A132197  2^n-1 written 2^n-1 times.
 * https://oeis.org/A132197
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^k – 1, where k is the smallest integer with Σ_{i=1}^{k}
 *            (2^i – 1) ≥ n. This yields the block that contains the n‑th term
 *            of the sequence “2^n‑1 written 2^n‑1 times”
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function pow2 is defined in the utilities; it returns 2^exp
   as a seq_t. */
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    idx_t k = 1;         /* block index (starting at 1) */
    seq_t cumulative = 0; /* cumulative number of terms so far */

    for (;; ++k)
    {
        /* size of the current block: 2^k – 1 */
        seq_t block = pow2(k);
        block -= 1;       /* 2^k – 1 */

        /* If the desired index falls within this block, return its
           value (which is 2^k – 1).  Note that n is cast to seq_t
           for the comparison. */
        if (cumulative + block >= (seq_t)n)
            return block;

        cumulative += block; /* advance to next block */
    }
}

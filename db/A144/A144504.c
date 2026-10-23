/*
 * A144504  3-transposable numbers.
 * https://oeis.org/A144504
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the concatenation of (n+1)/2 copies of 142857 if n is
 *            odd, else n/2 copies of 285714
 * Range:     u8 none, u16 none, u32 n <= 2, u64 n <= 6, u128 n <= 12,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Helper from utils to compute powers of 10. */
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;
    }

    /* Number of decimal digits in each block. */
    const idx_t block_len = 6;
    /* Pre‑compute 10^6 as a seq_t. */
    const seq_t ten_pow = pow10(block_len);  /* 1000000 in any size */

    seq_t base;
    idx_t reps;

    if (n & 1) {            /* odd n */
        base  = (seq_t)142857;
        reps  = (n + 1) >> 1;   /* (n+1)/2 */
    } else {                /* even n */
        base  = (seq_t)285714;
        reps  = n >> 1;          /* n/2 */
    }

    seq_t result = base;
    for (idx_t i = 1; i < reps; ++i) {
        /* Append another block: multiply by 10^6 and add the block. */
        result = result * ten_pow + base;
    }

    return result;
}

/*
 * A080584  A run of 3*2^n 0's followed by a run of 3*2^n 1's, for n=0, 1, 2,
 *          ...
 * https://oeis.org/A080584
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A080584
    The terms are formed by concatenating for each n = 0,1,2,...
    a block of 3·2ⁿ zeros followed by a block of 3·2ⁿ ones.
    This implementation simply walks through the blocks until the
    block containing the requested index is found and then decides
    whether the position lies in the zero‑half or the one‑half.   */
#include <stdint.h>

seq_t a(idx_t n)
{
    uint64_t start  = 0;   /* index of the start of the current block   */
    uint64_t half   = 3;   /* length of the zero‑half of the block      */
    uint64_t block  = 6;   /* total length of the block (zero + one)    */

    while (n >= start + block) {
        start += block;         /* move to next block                      */
        /* guard against overflow – for the test ranges this does not
           trigger, but makes the function safe for larger n.          */
        if (half > UINT64_MAX >> 1 || block > UINT64_MAX >> 1)
            break;
        half  <<= 1;            /* next block has twice the zero length     */
        block <<= 1;            /* and twice the total length              */
    }

    return (uint64_t)(n - start) < half ? (seq_t)0 : (seq_t)1;
}

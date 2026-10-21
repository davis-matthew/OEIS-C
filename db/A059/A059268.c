/*
 * A059268  Concatenate subsequences [2^0, 2^1, ..., 2^n] for n = 0, 1, 2, ...
 * https://oeis.org/A059268
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = 2^( n - t*(t+1)/2 ), where t = floor( (sqrt(1+8*n)-1)/2 )
 * Range:     u8 n <= 43, u16 n <= 151, u32 n <= 559, u64 n <= 2143,
 *            u128 n <= 8383, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    idx_t cum   = 0;   /* cumulative length of processed stages    */
    idx_t level = 0;   /* current stage index (0‑based)            */

    /* Find the stage that contains position n.  After the loop
       `cum` equals the total number of terms before this stage
       and `level` equals the index of this stage.            */
    while (cum + level + 1 <= n) {
        cum   += level + 1;   /* length of this stage   */
        level += 1;           /* move to next stage     */
    }

    /* Offset inside the found stage: 0 ≤ offset ≤ level          */
    idx_t offset = n - cum;

    /* Value is 2^offset.  The pow2 utility returns a seq_t          */
    return pow2(offset);
}

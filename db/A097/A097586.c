/*
 * A097586  Table T(n,n) read by rows: T(1,1)=0; then if n even
 *          T(n,1)=T(n-1,1)+2 and if n odd T(n,1)=T(n-2,1)+T(n-1,1)-1 then
 *          T(n,j)=T(n,j-1) + 2^floor((n+1)/2).
 * https://oeis.org/A097586
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = T(r,c) where T(n,1) is defined by T(1,1)=0; for even n:
 *            T(n,1)=T(n-1,1)+2; for odd n>1: T(n,1)=T(n-2,1)+T(n-1,1)-1. And
 *            T(n,j)=T(n,1)+(j-1)*2^floor((n+1)/2). a(n) reads rows of 10
 *            terms each
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n) {
    /* Determine row and column in the 10‑term rows. */
    idx_t row = (n - 1) / 10 + 1;
    idx_t col = (n - 1) % 10 + 1;

    /* Compute T(row,1) using the recurrence. */
    seq_t base = 0;                /* T(1,1) */
    if (row > 1) {
        seq_t prev1 = 0;           /* T(i-1,1) */
        seq_t prev2 = 0;           /* T(i-2,1) */
        seq_t curr;
        for (idx_t i = 2; i <= row; ++i) {
            if ((i & 1) == 0) {   /* even i */
                curr = prev1 + 2;
            } else {              /* odd i > 1 */
                curr = prev2 + prev1 - 1;
            }
            prev2 = prev1;
            prev1 = curr;
        }
        base = prev1;              /* T(row,1) */
    }

    /* Increment per column in the row. */
    seq_t step = pow2((row + 1) / 2);

    /* Final value for a(n). */
    return base + (seq_t)(col - 1) * step;
}

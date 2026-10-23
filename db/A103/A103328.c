/*
 * A103328  Triangle T(n, k) read by rows: binomial(2n, 2k+1).
 * https://oeis.org/A103328
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 38, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A103328:  Triangle T(n,k) = binomial(2n,2k+1), read by rows.
    The sequence a(n) is the flattened triangle:
    a(0)=0, a(1)=2, a(2)=0, a(3)=4, a(4)=4, a(5)=0, …      */
/*  We determine the row r and column k that contain the n‑th term,
    then evaluate binomial(2r,2k+1) using the provided binom() helper
    (which is defined for arbitrary indices).  Complexity is O(√n).  */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Find r such that r(r+1)/2 <= n < (r+1)(r+2)/2.  */
    idx_t r      = 0;
    idx_t offset = 0;                /* cumulative terms before row r */

    while (offset + r + 1 <= n) {
        r      += 1;
        offset += r;                 /* offset = r*(r+1)/2 after increment */
    }

    idx_t k = n - offset;            /* column within the row (0‑based) */
    /* Compute binomial(2*r, 2*k+1).  binom() returns seq_t.  */
    return binom((idx_t)(2 * r), (idx_t)(2 * k + 1));
}

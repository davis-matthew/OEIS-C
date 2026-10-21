/*
 * A082693  Pyramidal sequence built with powers of 2.
 * https://oeis.org/A082693
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   a(n) = 2 ^ ( r - min(j, 2*r - j) ) where r = ⌊√n⌋ and j = n -
 *            r^2 (offset inside the r‑th row of the pyramid)
 * Range:     u8 n <= 63, u16 n <= 255, u32 n <= 1023, u64 n <= 4095,
 *            u128 n <= 9995, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/*  Integer square root: floor(√n)  */
static inline idx_t isqrt_idx(idx_t n)
{
    idx_t low  = 0;
    idx_t high = n;
    idx_t r    = 0;           /* result */

    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);

        if (mid == 0) {      /* avoid division by zero */
            r = 0;
            low = 1;
            continue;
        }

        if (mid > n / mid) { /* mid*mid > n */
            high = mid - 1;
        } else {             /* mid*mid ≤ n */
            r = mid;
            low = mid + 1;
        }
    }
    return r;
}

/*  Compute 2^exp in seq_t, safe for all seq_t sizes */
static inline seq_t pow2_exp(idx_t exp)
{
    seq_t val = 1;
    for (idx_t i = 0; i < exp; ++i)
        val <<= 1;          /* shift by 1 per iteration – no UB */
    return val;
}

/*  Main sequence function */
seq_t a(idx_t n)
{
    idx_t r   = isqrt_idx(n);          /* row index */
    idx_t j   = n - r * r;             /* position within the row */
    idx_t dist = (j <= r) ? j : (2 * r - j);  /* distance to nearest edge */
    idx_t exp  = r - dist;             /* exponent of 2 */
    return pow2_exp(exp);
}

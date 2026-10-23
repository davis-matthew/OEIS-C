/*
 * A045755  8-fold factorials: a(n) = Product_{k=0..n-1} (8*k+1).
 * https://oeis.org/A045755
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=0..n-1} (8*k + 1) (OEIS A045755: 8‑fold
 *            factorials)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* for uint64_t (idx_t) */

seq_t a(idx_t n)
{
    seq_t prod = 1;          /* a(0) = 1 */
    for (idx_t k = 0; k < n; ++k)
    {
        /* (8*k + 1) fits in idx_t for all realistic n (<=10^4) */
        prod *= (seq_t)(8 * k + 1);
    }
    return prod;
}

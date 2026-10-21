/*
 * A225373  a(n) = 1 + Sum_{i=0..floor(n/2)} phi(n-2*i).
 * https://oeis.org/A225373
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Σ_{i=0..⌊n/2⌋} φ(n−2i)
 * Range:     u8 n <= 34, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   calc_phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Use the totient routine supplied by the build utilities. */
#include <stddef.h>
#include "../utils/calc_phi.h"

/* Compute A225373(n) for any non‑negative n. */
seq_t a(idx_t n)
{
    seq_t sum = 1;                      /* the leading +1 */
    idx_t half = n >> 1;                /* floor(n/2) */
    for (idx_t i = 0; i <= half; ++i) {
        idx_t k = n - (i << 1);         /* n − 2*i */
        seq_t phi_val;
        if (k == 0)                     /* φ(0) is defined as 0 */
            phi_val = 0;
        else
            phi_val = calc_phi((seq_t)k);
        sum += phi_val;
    }
    return sum;
}

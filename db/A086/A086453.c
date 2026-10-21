/*
 * A086453  Least difference between 5^n and a power of 2.
 * https://oeis.org/A086453
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min(5^n - 2^k, 2^(k+1) - 5^n) where k = floor(log₂(5^n))
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   bitlen_seq_t, pow2, power5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the minimal distance between 5^n and the nearest power of 2. */

#include <stddef.h>
#include "../utils/power5.h"
#include "../utils/pow2.h"
#include "../utils/bitlen_seq_t.h"

seq_t a(idx_t n)
{
    /* Compute 5^n. */
    seq_t p = power5(n);

    /* k = floor(log2(p)) = bitlen(p) - 1. */
    size_t bl = bitlen_seq_t(p);
    idx_t k = (idx_t)(bl - 1);

    /* Powers of two surrounding 5^n. */
    seq_t two_k  = pow2(k);
    seq_t two_k1 = pow2(k + 1);

    /* Distances to the surrounding powers. */
    seq_t diff_lower = p - two_k;   /* always >= 0 */
    seq_t diff_upper = two_k1 - p;  /* always >= 0 */

    return diff_lower < diff_upper ? diff_lower : diff_upper;
}

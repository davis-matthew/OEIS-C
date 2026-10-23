/*
 * A028679  Galois numbers for p=17; order of group AGL(n,17).
 * https://oeis.org/A028679
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 17^n * ∏_{k=0}^{n-1} (17^n - 17^k)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 5,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* Compute 17^n */
    seq_t pown = pow_uint(17ULL, n);
    seq_t prod = (seq_t)1;
    seq_t pow_k = (seq_t)1;            /* 17^k, starts with k = 0 */

    for (idx_t k = 0; k < n; ++k)
    {
        prod = prod * (pown - pow_k);  /* multiply by (17^n - 17^k) */
        pow_k = pow_k * (seq_t)17ULL;  /* advance to next power 17^(k+1) */
    }

    return pown * prod;                /* final multiplication by 17^n */
}

/*
 * A195459  a(n) = phi(3*n)/2.
 * https://oeis.org/A195459
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = phi(3*n)/2
 * Range:     u8 n <= 143, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/phi.h"


/* Return the n-th term of A195459. */
seq_t a(idx_t n)
{
    if (n == 0)            /* offset is 1 – guard for out‑of‑range */
        return 0;

    /* 3*n fits in idx_t for all allowed n */
    seq_t val = phi(3 * n);    /* φ(3*n) */
    return val / (seq_t)2;     /* divide by 2 */
}

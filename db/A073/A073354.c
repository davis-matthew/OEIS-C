/*
 * A073354  Binomial coefficient ( n, squarefree kernel(n) ).
 * https://oeis.org/A073354
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = binomial(n, rad(n)) where rad(n) is the square‑free
 *            kernel of n (product of distinct prime divisors)
 * Range:     u8 n <= 11, u16 n <= 17, u32 n <= 39, u64 n <= 67, u128 n <= 131,
 *            big any n
 * Helpers:   binomial, squarefree_kernel
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/squarefree_kernel.h"

/* Implementation assumes that idx_t and seq_t are defined by the build system. */

/* External linkage as required. */
seq_t a(idx_t n)
{
    if (n == 0)          /* not used, but safeguard */
        return 0;
    idx_t rad = squarefree_kernel(n);
    return binomial(n, rad);
}

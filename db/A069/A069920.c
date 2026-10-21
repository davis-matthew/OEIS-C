/*
 * A069920  Number of noninvertible n X n matrices mod 6 (i.e., over the ring
 *          Z_6).
 * https://oeis.org/A069920
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6^(n^2) - |GL(n,2)| * |GL(n,3)| * with |GL(n,p)| =
 *            ∏_{i=0}^{n-1} (p^n - p^i) for prime p. * This implementation
 *            uses fast exponentiation for 6^(n^2) * and a simple accumulation
 *            for the GL counts. All * arithmetic is performed on seq_t; for
 *            arbitrary‑precision * builds the operator* is defi...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 7,
 *            big any n
 * Helpers:   gl_count
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/gl_count.h"

/* Fast exponentiation for seq_t */
static seq_t pow_seq_t(seq_t base, uint64_t exp)
{
    seq_t result = 1;
    seq_t b = base;
    while (exp)
    {
        if (exp & 1ULL) result *= b;
        b *= b;
        exp >>= 1ULL;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* 6^(n^2) */
    uint64_t exp = (uint64_t)n * (uint64_t)n;
    seq_t six_pow = pow_seq_t((seq_t)6, exp);

    /* |GL(n,2)| and |GL(n,3)| */
    seq_t gl2 = gl_count(n, 2ULL);
    seq_t gl3 = gl_count(n, 3ULL);

    seq_t invertible = gl2 * gl3;
    return six_pow - invertible;
}

/*
 * A069731  Number of unicursal planar maps with n edges rooted at a vertex of
 *          odd valency (unicursal means that exactly two vertices are of odd
 *          valency; there is an Eulerian path).
 * https://oeis.org/A069731
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^(n-2) * Catalan(n+1), with a(1) = 1, a(2) = 5, …
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Validated: partial, pass, 23 terms through n = 23 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS sequence A069731.
 * Uses the binomial helper binom(idx_t n, idx_t k) which returns a seq_t.
 * Handles the special case n == 1 where 2^(n-2) = 1/2 by dividing an extra factor 2 later. */
#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* n >= 1 by definition of the sequence */
    idx_t m = n + 1;                 /* m = n+1 */
    idx_t twom = 2 * m;              /* 2*(n+1) */

    /* Catalan(m) = binom(2m, m) / (m+1) */
    seq_t bin = binomial(twom, m);
    seq_t catalan = bin / (m + 1);

    seq_t result = catalan;

    if (n == 1) {
        /* 2^(n-2) = 1/2, so divide by 2 after using Catalan(2)=2 */
        result /= 2;
    } else if (n > 2) {
        /* Compute 2^(n-2) by repeated doubling */
        seq_t pow2 = 1;
        for (idx_t i = 0; i < n - 2; ++i) {
            pow2 *= 2;
        }
        result *= pow2;
    }
    /* n == 2 => 2^(0) = 1, nothing to do */

    return result;
}

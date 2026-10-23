/*
 * A119349  Denominators of expansion of Jacobi nome q in parameter m.
 * https://oeis.org/A119349
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 *  OEIS A119349 – Denominators of the expansion of Jacobi nome q in parameter m.
 *  Let b(0)=0 and, for n>0,
 *      b(n) = 4 + ⌊(n-1)/8⌋·32 + Σ_{k=0}^{((n-1) mod 8)-1} c_k
 *  where c = {1, 5, 1, 8, 1, 5, 1, 10}.
 *  Then a(n) = 2^b(n).
 *
 *  The sequence starts: 1, 16, 32, 1024, 2048, 524288, …
 *
 *  This implementation computes the exponent b(n) in O(1) time using the
 *  periodic pattern of differences and returns 2^b(n) via pow2().
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)               /* a(0) = 1 = 2^0 */
        return pow2(0);

    /* Number of increments after the first term */
    idx_t m   = n - 1;
    idx_t cycles = m / 8;
    idx_t rem    = m % 8;

    /* Base exponent after adding full cycles */
    idx_t exp = 4 + cycles * 32;

    /* Partial cycle contributions */
    static const idx_t cycle[8] = { 1, 5, 1, 8, 1, 5, 1, 10 };
    for (idx_t i = 0; i < rem; ++i)
        exp += cycle[i];

    return pow2(exp);   /* a(n) = 2^exp */
}

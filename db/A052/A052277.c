/*
 * A052277  a(n) = (4n+2)!/2^(2n+1).
 * https://oeis.org/A052277
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n+2)! / 2^(2*n+1)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 5, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: compute odd part of (4n+2)! and multiply back the remaining 2-powers. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* m = 4n+2  is the top of the factorial. */
    idx_t m = 4 * n + 2;

    /* Compute the odd part of m! :
       odd = Π_{i=1..m} (i after removing all factors of 2).
    */
    seq_t odd = 1;
    for (idx_t i = 1; i <= m; ++i) {
        idx_t x = i;
        while ((x & 1u) == 0)         /* strip all powers of 2 from x   */
            x >>= 1;
        odd *= (seq_t)x;              /* multiply the odd part of i      */
    }

    /* Total exponent of 2 in m!  (Legendre’s formula). */
    idx_t exp2 = 0;
    for (idx_t p = 2; p <= m; p <<= 1) {
        exp2 += m / p;
    }

    /* Remaining exponent after dividing by 2^(2n+1). */
    idx_t k = exp2 - (2 * n + 1);

    /* Multiply by the remaining powers of 2. */
    seq_t result = odd;
    if (k > 0) {
        result <<= (unsigned int)k;   /* shift by k bits (k is safe for values that fit in seq_t) */
    }

    return result;
}

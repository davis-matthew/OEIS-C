/*
 * A081480  Consider the mapping f(a/b) = (a^2 +b^2)/(a+b). Taking a =1, b = 2
 *          to start with and carrying out this mapping repeatedly on each new
 *          (reduced) rational number gives the following sequence
 *          1/2,5/3,17/4,305/21,... Sequence contains the denominators.
 * https://oeis.org/A081480
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = denominator of the reduced fraction obtained by
 *            repeatedly applying f(p/q) = (p²+q²)/(p+q) to the starting
 *            rational 1/2. The first term a(1) = 2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 8, u128 n <= 9,
 *            big any n
 * Helpers:   seq_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>               /* for size_t */
#include <stdint.h>               /* for uintptr_t */
#include "../utils/seq_gcd.h"     /* seq_gcd() */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined, harmless */

    /* Start from the first unreduced fraction 1/2. */
    seq_t p = (seq_t)1;
    seq_t q = (seq_t)2;

    if (n == 1) return q;                /* return denominator 2 */

    for (idx_t i = 2; i <= n; ++i) {
        /* Compute the next fraction before reduction. */
        seq_t u = p * p + q * q;         /* numerator */
        seq_t v = p + q;                 /* denominator */

        /* Reduce the fraction and store the result for the next step. */
        seq_t g = seq_gcd(u, v);
        p = u / g;
        q = v / g;
    }

    return q;                            /* denominator of the nth term */
}

/*
 * A071716  Expansion of (1+x^2*C)*C, where C = (1-(1-4*x)^(1/2))/(2*x) is
 *          g.f. for Catalan numbers, A000108.
 * https://oeis.org/A071716
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Catalan(n) + Catalan(n-1) for n >= 2, a(0) = a(1) = 1
 *            (A071716)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 19, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Validated: partial, pass, 29 terms through n = 28 (2026-11-01)
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    /* Handle the first two terms */
    if (n == 0 || n == 1)
        return (seq_t)1;

    /* Compute Catalan numbers for n and n-1 using the utility function */
    seq_t c_n  = catalan(n);
    seq_t c_n1 = catalan(n - 1);

    /* Sum them to get the desired term */
    return c_n + c_n1;
}

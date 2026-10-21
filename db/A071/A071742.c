/*
 * A071742  Expansion of (1+x^4*C)*C, where C = (1-(1-4*x)^(1/2))/(2*x) is
 *          g.f. for Catalan numbers, A000108.
 * https://oeis.org/A071742
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Catalan(n) + (n>=4 ? Catalan(n-3) : 0)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  (Catalan(n) is the nth Catalan number.) */

#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    /* For n = 0,1,2,3 we only need the nth Catalan number */
    if (n < 4) {
        return catalan(n);
    }
    /* For n >= 4: a(n) = Catalan(n) + Catalan(n-3) */
    return catalan(n) + catalan(n - 3);
}

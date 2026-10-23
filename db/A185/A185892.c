/*
 * A185892  Trajectory of x^14+x^13+...x+1 under the map (see A185544) defined
 *          in the Comments.
 * https://oeis.org/A185892
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   bitlen_seq_t, next_poly
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the trajectory of the polynomial x^14+x^13+…+x+1 in GF(2)[x].
   Each iteration applies the map described in A185544:
        f(0)=0  ⇒  f := f/x
        f(0)=1  ⇒  f := ((x²+1)f + 1) / x
   a(n) (offset 1) is the binary coefficient string of the polynomial
   after n‑1 iterations, interpreted as a decimal integer.  */

#include <stddef.h>                  /* for size_t */
#include "../utils/bitlen_seq_t.h"   /* bitlen_seq_t() */
#include "../utils/next_poly.h"

/* a(n) (offset 1) */
seq_t a (idx_t n)
{
    /* starting polynomial x^14 + … + x + 1  (15 consecutive 1‑coefficients) */
    seq_t f = 0;
    for (idx_t i = 0; i < 15; ++i)
        f |= ((seq_t)1 << i);

    for (idx_t i = 1; i < n; ++i)      /* apply n-1 steps */
        f = next_poly (f);

    /* Convert the binary coefficient string of f to a decimal integer */
    seq_t val = 0;
    size_t bits = bitlen_seq_t (f);
    for (size_t i = bits; i > 0; --i) {
        val = val * (seq_t)10 + ((f >> (i - 1)) & (seq_t)1);
    }
    return val;
}

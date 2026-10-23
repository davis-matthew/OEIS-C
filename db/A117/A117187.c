/*
 * A117187  Expansion of (1+x)c(x^2)/((1-x^2*c(x^2))sqrt(1-4x^2)), c(x) the
 *          g.f. of A000108.
 * https://oeis.org/A117187
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = binomial(2*floor(n/2)+2, floor(n/2)+1) -
 *            catalan(floor(n/2)+1)
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 29, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   binomial, catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses binomial and catalan utility functions from the utils library */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    idx_t k = n >> 1;                  /* floor(n/2) */
    /* Compute binomial(2*k+2, k+1) */
    idx_t top  = (k << 1) + 2;          /* 2*k + 2 */
    idx_t bot  = k + 1;                 /* k + 1 */

    seq_t bin   = binomial(top, bot);   /* binomial(2*k+2, k+1) */
    seq_t cat   = catalan(k + 1);       /* catalan(k+1) */

    return bin - cat;
}

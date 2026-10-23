/*
 * A158681  Wiener indexes of the complete binary trees with n levels, root
 *          being at level 0.
 * https://oeis.org/A158681
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+4)*2^(n+1) + (n-2)*2^(2n+2)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 13, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

/* External linkage – computes the Wiener index of a complete
 * binary tree with `n` levels (root at level 0).  The OEIS
 * offset here is 1, so n>=1. */
seq_t a(idx_t n)
{
    /* Guard against the degenerate case n==0 – it does not
     * appear in the OEIS sequence but protects against
     * undefined behaviour if called. */
    if (n == 0)
        return 0;

    /* For n==1 the formula gives (5*4)+(-1)*16 = 4.  The
     * unsigned arithmetic would underflow for (n-2), so
     * handle this case explicitly. */
    if (n == 1)
        return (seq_t)4;

    /* Use idx_t for the exponents passed to pow2, as it
     * expects an unsigned 64‑bit integer.  All intermediate
     * calculations use seq_t to accommodate arbitrary
     * precision. */
    idx_t exp1 = n + 1;
    idx_t exp2 = 2 * n + 2;

    seq_t tn   = (seq_t)n;          /* promote n to seq_t */
    seq_t term1 = (tn + 4) * pow2(exp1);
    seq_t term2 = (tn - 2) * pow2(exp2);

    return term1 + term2;
}

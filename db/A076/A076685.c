/*
 * A076685  a(n) = max(core(n),phi(n)) where core(n) is the squarefree part of
 *          n.
 * https://oeis.org/A076685
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max(core(n), phi(n)) where core(n) is the squarefree part
 *            of n, defined as n divided by the largest square dividing n
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   core_squarefree_part, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses the provided phi routine and an
   inline factorisation to compute the squarefree part. */

#include "../utils/phi.h"
#include "../utils/core_squarefree_part.h"

seq_t a(idx_t n)
{
    if (n == 0) /* undefined, but return 0 to avoid UB */
        return (seq_t)0;

    seq_t core = core_squarefree_part(n);
    seq_t phi_val = phi((uint64_t)n);

    return (core > phi_val) ? core : phi_val;
}

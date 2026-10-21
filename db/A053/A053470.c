/*
 * A053470  a(n) is the cototient of n (A051953) iterated twice.
 * https://oeis.org/A053470
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = cototient(cototient(n)) (cototient(n) = n - φ(n))
 * Range:     u8 n <= 334, u16 n <= 9285, u32 n <= 9285, u64 n <= 9285,
 *            u128 n <= 9285, big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"

/* Compute the cototient of an integer n: n - φ(n)   (φ is provided in phi.h) */
static seq_t cototient(idx_t n)
{
    /* φ(n) is at most n, so the subtraction fits in seq_t. */
    seq_t ph = phi(n);
    return (seq_t)n - ph;
}

/* Required sequence function */
seq_t a(idx_t n)
{
    /* Sequence is defined for n ≥ 1.  For n == 0 the value is undefined,
       but we return 0 to avoid unsafe accesses. */
    if (n == 0)
        return (seq_t)0;

    /* First cototient */
    seq_t first = cototient(n);
    if (first == 0)              /* a(1) and all n where n-φ(n)=1 give 0 */
        return (seq_t)0;

    /* Second cototient */
    idx_t m = (idx_t)first;     /* first ≤ n-1 ≤ UINT64_MAX, safe conversion */
    return cototient(m);
}

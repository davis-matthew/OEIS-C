/*
 * A120989  Level of the first leaf (in preorder traversal) of a binary tree,
 *          summed over all binary trees with n edges. A binary tree is a
 *          rooted tree in which each vertex has at most two children and each
 *          child of a vertex is designated as its left or right child.
 * https://oeis.org/A120989
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n*(7*n+13)*binomial(2*n+1,n)/((n+2)*(n+3)*(n+4))
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 11, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Return a(0)=0 as a convenient fallback. */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Handle the offset: the sequence is defined only for n ≥ 1. */
    if (n == 0) return 0;

    /* Compute the central binomial coefficient C(2n+1,n). */
    idx_t top   = 2 * n + 1;
    seq_t bin   = binomial(top, n);

    /* Compute numerator: bin * (2*n) * (7*n+13). */
    seq_t num   = bin * (seq_t)(2 * n);
    num         = num * (seq_t)(7 * n + 13);

    /* Compute denominator: (n+2)*(n+3)*(n+4). */
    seq_t den   = (seq_t)(n + 2);
    den         = den * (seq_t)(n + 3);
    den         = den * (seq_t)(n + 4);

    /* Exact division – the result is an integer. */
    return num / den;
}

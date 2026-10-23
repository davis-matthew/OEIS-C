/*
 * A192832  Molecular topological indices of the lattice graphs.
 * https://oeis.org/A192832
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n^2*(n+1)*(n-1)^2
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 64, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: a(1) = 0 by the formula itself. */

seq_t a(idx_t n)
{
    /* Use the plain algebraic formula.  All intermediate
       products remain within the bounds of the target type for
       any n that keeps the final result within seq_t
       (i.e., up to 2^64 on a 64‑bit build). */
    seq_t nn = (seq_t)n;

    seq_t result = (seq_t)4;           /* 4 */
    result = result * nn;              /* 4*n */
    result = result * nn;              /* 4*n^2 */
    result = result * (nn + (seq_t)1); /* 4*n^2*(n+1) */
    result = result * (nn - (seq_t)1); /* 4*n^2*(n+1)*(n-1) */
    result = result * (nn - (seq_t)1); /* 4*n^2*(n+1)*(n-1)^2 */

    return result;
}

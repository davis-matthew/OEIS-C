/*
 * A051170  a(n) = T(n,5), array T as in A051168; a count of Lyndon words;
 *          aperiodic necklaces with 5 black beads and n-5 white beads.
 * https://oeis.org/A051170
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor( C(n,5) / n )
 * Range:     u8 n <= 10, u16 n <= 25, u32 n <= 287, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* for uintptr_t compatibility, idx_t is defined by the build */

/* Computes a(n) for a(n) = floor( binomial(n,5) / n ) */
seq_t a(idx_t n)
{
    /* a(n) is undefined for n < 5; return 0 for those values */
    if (n < 5)
        return 0;

    /* Compute binomial(n,5) multiplicatively to avoid overflow */
    seq_t binom = 1;
    idx_t i;
    for (i = 1; i <= 5; ++i) {
        binom = binom * (seq_t)(n + 1 - i) / (seq_t)i;
    }

    /* Integer division truncates toward zero, i.e., floor for positives */
    return binom / (seq_t)n;
}

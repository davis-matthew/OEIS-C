/*
 * A060344  For n >= 2, let N_n denote the set of all unipotent
 *          upper-triangular real n X n matrices A such that for every
 *          k=1,2,...,n-1 the minor of A with rows 1,2,...,k and columns
 *          n-k+1,...,n is nonzero. a(n) is the number of connected components
 *          of N_n.
 * https://oeis.org/A060344
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*2^(n-1) for n > 5 (with a(2)=2, a(3)=6, a(4)=20,
 *            a(5)=52)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* values for the offset range */
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)6;
    if (n == 4) return (seq_t)20;
    if (n == 5) return (seq_t)52;

    /* for n >= 6: compute 3*2^(n-1) */
    seq_t result = (seq_t)1;
    for (idx_t i = 1; i < n; ++i)
        result *= (seq_t)2;          /* result = 2^(n-1) */
    result *= (seq_t)3;              /* result = 3*2^(n-1) */
    return result;
}

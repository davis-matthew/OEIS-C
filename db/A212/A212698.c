/*
 * A212698  Main transitions in systems of n particles with spin 3/2.
 * https://oeis.org/A212698
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n*4^(n-1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 29, u128 n <= 61,
 *            big any n
 * Helpers:   pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Use the provided pow4 helper for exponentiation of 4 */
#include "../utils/pow4.h"

/* External linkage as required */
seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1.
       For n == 0 the value is undefined; return 0 if encountered. */
    if (n == 0) return 0;

    /* Compute 4^(n-1) first */
    seq_t val = pow4(n - 1);

    /* Multiply by 3 and then by n */
    val *= (seq_t)3;
    val *= (seq_t)n;

    return val;
}

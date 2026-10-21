/*
 * A061206  a(n) = total number of occurrences of the consecutive pattern 1324
 *          in all permutations of [n+3].
 * https://oeis.org/A061206
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(n+3)!/24
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* 0 is outside the offset but safe */

    /* Compute (n+3)!  */
    idx_t m = n + 3;
    seq_t fact = (seq_t)1;
    for (idx_t i = 2; i <= m; ++i)
        fact = fact * (seq_t)i;              /* works for integer types and big‑int classes */

    /* Divide by 24 and multiply by n */
    fact = fact / (seq_t)24;                 /* exact division */
    return fact * (seq_t)n;
}

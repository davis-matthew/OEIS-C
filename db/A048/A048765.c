/*
 * A048765  Smallest factorial >= n.
 * https://oeis.org/A048765
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the smallest factorial m! such that m! >= n
 * Range:     u8 n <= 120, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: compute factorial incrementally until it reaches or exceeds n. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* cast the input to the sequence type to avoid mixing signed/unsigned */
    const seq_t target = (seq_t)n;
    seq_t fact = (seq_t)1;   /* starts with 0! = 1 */

    /* Find the first factorial >= target */
    for (seq_t k = (seq_t)1; fact < target; ++k)
    {
        fact *= (k + (seq_t)1);   /* multiply by next integer */
    }

    return fact;
}

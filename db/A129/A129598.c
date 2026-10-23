/*
 * A129598  a(n) = n * A111089(n).
 * https://oeis.org/A129598
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS formula: a(n) = n * A111089(n) = n * (largest prime factor of n) */
/* For n=1 the largest prime factor is undefined, so a(1) is set to 2. */

#include "../utils/gpf.h"

seq_t a(idx_t n)
{
    if (n == 0)   /* undefined, but safe to return 0 */
        return 0;

    if (n == 1)   /* special case from the sequence definition */
        return 2;

    /* gpf returns the largest prime factor for n <= 2^64 */
    uint64_t lp = gpf((uint64_t)n);
    return (seq_t)n * (seq_t)lp;
}

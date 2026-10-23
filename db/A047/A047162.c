/*
 * A047162  Number of nonempty subsets of {1,2,...,n} in which exactly 2/3 of
 *          the elements are <= n/2.
 * https://oeis.org/A047162
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 21, u32 n <= 38, u64 n <= 73, u128 n <= 143,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = Sum_{k>=1} C(floor(n/2), 2*k) * C(ceil(n/2), k)
 * The computation uses the supplied binomial(a,b) helper, which returns
 * the exact binomial coefficient as a seq_t value.  The sum is accumulated
 * in a seq_t variable and returned.  Everything is done with unsigned
 * integer arithmetic only; no floating point or dynamic memory allocation
 * is required.
 */

#include <stdint.h>
#include "../utils/binomial.h"   /* provides seq_t binomial(idx_t n, idx_t k); */

seq_t a(idx_t n)
{
    if (n == 0)                          /* offset is 1, behaviour for n<1 is irrelevant */
        return (seq_t)0;

    /* floor(n/2) and ceil(n/2) */
    idx_t low  = n >> 1;                 /* floor(n/2) */
    idx_t high = n - low;                /* ceil(n/2) */

    /* k must satisfy 2*k <= low and k <= high */
    idx_t kmax = low >> 1;               /* floor(low/2) */
    if (kmax > high) kmax = high;        /* k <= high */

    seq_t sum = 0;
    for (idx_t k = 1; k <= kmax; ++k) {
        seq_t b1 = binomial(low,  2*k);
        seq_t b2 = binomial(high, k);
        sum += b1 * b2;
    }
    return sum;
}

/*
 * A047193  Number of nonempty subsets of {1,2,...,n} in which exactly 1/2 of
 *          the elements are <= n/3.
 * https://oeis.org/A047193
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(n, floor(n/3)) - 1
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*                                Excludes the empty set */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* For n < 1 the behaviour is unspecified; return 0 */
    if (n == 0) return 0;

    idx_t k = n / 3;               /* floor(n/3) */
    seq_t r = binomial(n, k);      /* binomial coefficient */
    return r - (seq_t)1;           /* subtract the empty subset */
}

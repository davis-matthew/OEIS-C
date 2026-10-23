/*
 * A048038  Number of nonempty subsets of {1,2,...,n} in which exactly 1/2 of
 *          the elements are <= (n+1)/3.
 * https://oeis.org/A048038
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binom(n, floor((n+1)/3)) - 1
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    idx_t t = (n + 1) / 3; /* floor((n+1)/3) */
    seq_t val = binomial(n, t);
    return val - ((seq_t)1);
}

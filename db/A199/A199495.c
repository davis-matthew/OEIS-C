/*
 * A199495  Number of permutations of [n] starting and ending with an odd
 *          number.
 * https://oeis.org/A199495
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0, a(1)=1, a(2)=0. For n>=3, a(n) = ((n+1)/2) * ((n-1)/2) *
 *            (n-2)!
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* the above is equivalent to 2*A152887(n-2) for n>=2 */

#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)0;

    idx_t odds = (n + 1) / 2;          /* ceil(n/2) */
    seq_t term = (seq_t)odds * (seq_t)(odds - 1);
    seq_t fact = factorial(n - 2);
    return term * fact;
}

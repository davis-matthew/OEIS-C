/*
 * A047198  Number of nonempty subsets of {1,2,...,n} in which exactly 1/5 of
 *          the elements are <= n/3.
 * https://oeis.org/A047198
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{r=1}^{R} binom(⌊n/3⌋, r) * binom(n - ⌊n/3⌋, 4r), with
 *            R = min(⌊n/3⌋, ⌊(n - ⌊n/3⌋)/4⌋)
 * Range:     u8 n <= 10, u16 n <= 20, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*=====================================================================*/
#include "../utils/binom.h"

/* Return number of nonempty subsets of {1,…,n} in which exactly
   one–fifth of the elements are <= n/3. */
seq_t a(idx_t n)
{
    idx_t low   = n / 3;             /* floor(n/3) */
    idx_t high  = n - low;           /* elements > n/3 */
    idx_t rmax  = low;               /* upper bound from low side */
    idx_t h4    = high >> 2;         /* high / 4 */
    if (h4 < rmax) rmax = h4;        /* adjust upper bound */

    seq_t sum = 0;
    for (idx_t r = 1; r <= rmax; ++r) {
        /* Choose r elements from the low part and 4r from the high part */
        seq_t lowWays  = binom(low, r);
        seq_t highWays = binom(high, 4 * r);
        sum += lowWays * highWays;   /* multiplication is safe in uintXX_t */
    }
    return sum;
}

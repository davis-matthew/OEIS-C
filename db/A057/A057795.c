/*
 * A057795  Sum k!, where sum is over positive integers k <= n with gcd(k,n) =
 *          1.
 * https://oeis.org/A057795
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{1≤k≤n, gcd(k,n)=1} k!
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   gcd_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd_idx.h"

/* Return the value a(n) of OEIS sequence A057795.
 * Sum of factorials of all positive integers k ≤ n which are coprime to n. */
seq_t a(idx_t n)
{
    if (n == 0)                /* out of the OEIS index range, but safe */
        return 0;

    seq_t sum  = 0;
    seq_t fact = 1;            /* factorial accumulator */

    for (idx_t k = 1; k <= n; ++k) {
        fact *= (seq_t)k;                        /* compute k! */
        if (gcd_idx(k, n) == 1)                 /* add only if coprime */
            sum += fact;
    }
    return sum;
}

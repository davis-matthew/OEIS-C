/*
 * A220114  Largest k >= 0 such that k = n - x - y where n = x*y, x > 0, y >
 *          0, or -1 if no such k exists.
 * https://oeis.org/A220114
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n - min_{d | n} (d + n/d), d>0
 * Range:     i8 n <= 80, i16 n <= 80, i32 n <= 80, i64 n <= 80, i128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   (equivalently a(n) = max{ n - x - y | x>0, y>0, x*y=n }) */

#include <stdint.h>

/* Compute a(n).  The loop iterates over all divisors d of n,
   using the condition d <= n/d to stay below the integer square root
   without floating‑point arithmetic.  This works for any n >= 1. */
seq_t a(idx_t n)
{
    if (n == 0)                /* sequence only defined for n >= 1 */
        return (seq_t)-1;

    /* A trivial bound: the factor pair (1,n) gives sum n+1. */
    seq_t min_sum = (seq_t)n + 1;

    /* Iterate over all d <= n/d.  The comparison n/d prevents
       overflow even when n is close to the maximum value of idx_t. */
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d != 0)
            continue;

        idx_t other = n / d;
        seq_t sum = (seq_t)d + (seq_t)other;
        if (sum < min_sum)
            min_sum = sum;
    }

    return (seq_t)n - min_sum;
}

/*
 * A121937  a(n) = least m >= 2 such that (n mod m) > (n+2 mod m).
 * https://oeis.org/A121937
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least m ≥ 2 such that n mod m > (n+2) mod m
 * Range:     u8 n <= 539, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalently: the smallest integer m ≥ 3 with n mod m ≥ m-2. */

#include <stdint.h>
#include <stdbool.h>

/* compute A121937 */
seq_t a(idx_t n)
{
    /* n ≥ 1.  For n = 1 the answer is 3, which is not covered by the loop
       below that starts at m = 3 and goes up to n+1.  Treat this special case. */
    if (n < 2)
        return (seq_t)3;

    for (idx_t m = 3; m <= n + 1; ++m)
    {
        idx_t r = n % m;           /* r = n mod m   */
        if (r >= m - 2)            /* (n mod m) >= m-2  */
            return (seq_t)m;
    }

    /* The loop always finds a value for n ≥ 2, but return n+1 as a safe
       fallback for completeness. */
    return (seq_t)(n + 1);
}

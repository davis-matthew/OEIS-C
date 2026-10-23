/*
 * A159074  Sum of the k in the range 1<=k<=n such that set of proper divisors
 *          of k is not a subset of the set of proper divisors of n.
 * https://oeis.org/A159074
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + sum of k (2 ≤ k ≤ n) for which some proper divisor of
 *            k is not a divisor of n. This is the same as “set of proper
 *            divisors of k is not a subset of the set of proper divisors of
 *            n”
 * Range:     u8 n <= 28, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses only integer arithmetic and works for any
   seq_t / idx_t type provided by the build.  It runs in O(n*sqrt(n))
   which is well below the limits for n ≤ 10000. */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)  /* undefined; return 0 for safety */
        return (seq_t)0;

    /* k = 1 is always counted, so start sum with 1. */
    seq_t sum = (seq_t)1;

    for (idx_t k = 2; k <= n; ++k)
    {
        bool fail = false;               /* fails subset condition? */

        /* Loop over proper divisors d of k (exclude 1, include k only
           if it appears as d1 or d2 with d < k). */
        for (idx_t d = 2; d * d <= k && !fail; ++d)
        {
            if (k % d == 0)
            {
                /* d is a proper divisor (d < k because d ≥ 2). */
                if (n % d != 0)
                {
                    fail = true;          /* divisor d not dividing n */
                    break;
                }

                idx_t d2 = k / d;        /* complementary divisor */
                if (d2 != d)             /* avoid double‑counting square roots */
                {
                    if (n % d2 != 0)
                    {
                        fail = true;      /* divisor d2 not dividing n */
                        break;
                    }
                }
            }
        }

        if (fail)
            sum += (seq_t)k;
    }

    return sum;
}

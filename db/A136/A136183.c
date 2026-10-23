/*
 * A136183  a(n) = Sum_{k=1..d(n)-1} lcm(b(k), b(k+1)), where b(k) is the k-th
 *          positive divisor of n and d(n) = number of positive divisors of n.
 * https://oeis.org/A136183
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1}^{d(n)-1} lcm(b(k),b(k+1)) where b(k) are the
 *            divisors of n in increasing order
 * Range:     u8 n <= 59, u16 n <= 3779, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* sequence defined for n ≥ 1 */

    /* A safe upper bound on the number of divisors of n ≤ 10000 is far below 2000. */
    const idx_t MAX_DIVISORS = 2000;
    idx_t divisors[MAX_DIVISORS];
    idx_t dcnt = 0;

    /* Collect all positive divisors of n. */
    for (idx_t i = 1; i * i <= n; ++i)
    {
        if (n % i == 0)
        {
            divisors[dcnt++] = i;
            if (i != n / i)
                divisors[dcnt++] = n / i;
        }
    }

    /* Sort the divisors in ascending order. */
    for (idx_t i = 0; i < dcnt; ++i)
    {
        for (idx_t j = i + 1; j < dcnt; ++j)
        {
            if (divisors[j] < divisors[i])
            {
                idx_t t = divisors[i];
                divisors[i] = divisors[j];
                divisors[j] = t;
            }
        }
    }

    /* Sum the LCMs of consecutive divisors. */
    seq_t sum = 0;
    for (idx_t i = 0; i + 1 < dcnt; ++i)
    {
        seq_t a1 = (seq_t)divisors[i];
        seq_t a2 = (seq_t)divisors[i + 1];
        seq_t g  = gcd(a1, a2);          /* gcd from the utilities */
        seq_t l  = (a1 / g) * a2;        /* lcm(a1,a2) */
        sum += l;
    }

    return sum;
}

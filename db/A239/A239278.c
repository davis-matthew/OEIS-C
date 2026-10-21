/*
 * A239278  Smallest k > 1 such that n*(n+1)*...*(n+k-1) /
 *          (n+(n+1)+...+(n+k-1)) is an integer.
 * https://oeis.org/A239278
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k>1 such that n*(n+1)*...*(n+k-1) /
 *            (n+(n+1)+...+(n+k-1)) is an integer
 * Range:     u8 n <= 9012, u16 n <= 9012, u32 n <= 9012, u64 n <= 9012,
 *            u128 n <= 9012, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* We search for the minimal k>1 satisfying the divisibility
       condition.  Since k stays very small (2,3,5,7,9 for n ≤ 10000),
       this brute‑force search is fast.  To avoid overflow on 64‑bit
       builds, the product is reduced modulo the sum in each step. */
    for (idx_t k = 2; ; ++k)
    {
        /* Compute the sum of the k consecutive terms */
        unsigned long long sum =
            (unsigned long long)k * (unsigned long long)n +
            ((unsigned long long)k * (k - 1) / 2);

        /* Compute the product modulo the sum */
        unsigned long long prod_mod = 1;
        for (idx_t i = 0; i < k; ++i)
        {
            unsigned long long val = (unsigned long long)n + i;
            prod_mod = (prod_mod * val) % sum;
            if (prod_mod == 0) break;   /* early exit if already divisible */
        }

        if (prod_mod == 0)
            return (seq_t)k;
    }
}

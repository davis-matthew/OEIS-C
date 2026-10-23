/*
 * A168065  If n = Product p(k)^e(k) then a(n) = (Product (p(k)+1)^e(k) +
 *          Product (p(k)-1)^e(k))/2, a(1) = 1.
 * https://oeis.org/A168065
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (∏ (p+1)^e + ∏ (p-1)^e) / 2 for n = ∏ p^e, a(1)=1
 * Range:     u8 n <= 47, u16 n <= 2047, u32 n <= 9975, u64 n <= 9975,
 *            u128 n <= 9975, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* a(1) is the special case */
    if (n == 1)
        return (seq_t)1;

    /* Use uint64_t for the intermediate integer arithmetic.
       seq_t is big enough to hold the result for all n in the test set. */
    uint64_t nn   = (uint64_t)n;
    seq_t prod_plus  = (seq_t)1;
    seq_t prod_minus = (seq_t)1;
    uint64_t temp = nn;

    /* Trial‑division factorisation – fast enough for n ≤ 10 000. */
    for (uint64_t p = 2; p * p <= temp; ++p)
    {
        if (temp % p == 0)
        {
            int e = 0;
            while (temp % p == 0)
            {
                temp /= p;
                ++e;
            }
            seq_t pp = (seq_t)p + 1;   /* (p+1)^e part   */
            seq_t pm = (seq_t)p - 1;   /* (p-1)^e part   */
            for (int k = 0; k < e; ++k)
            {
                prod_plus  *= pp;
                prod_minus *= pm;
            }
        }
    }

    /* Remaining prime factor (if any) */
    if (temp > 1)
    {
        seq_t pp = (seq_t)temp + 1;
        seq_t pm = (seq_t)temp - 1;
        prod_plus  *= pp;
        prod_minus *= pm;
    }

    /* a(n) = (prod_plus + prod_minus) / 2 */
    return (prod_plus + prod_minus) / 2;
}

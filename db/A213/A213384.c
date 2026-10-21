/*
 * A213384  Expansion of phi(-q)^3 in powers of q where phi() is a Ramanujan
 *          theta function.
 * https://oeis.org/A213384
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of q^n in (∑_{k∈ℤ} (-1)^k q^{k^2})^3
 *            (expanded as: a(n) = ∑_{r1,r2,r3≥0, r1²+r2²+r3²=n}
 *            2^{#positive(ri)} * (-1)^{r1+r2+r3} )
 * Range:     i8 n <= 88, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root using only integer arithmetic:
   returns floor(sqrt(x)) for 0 ≤ x ≤ 2^64-1.          */
static inline seq_t isqrt_u64(uint64_t x)
{
    uint64_t low = 0;
    uint64_t high = 1ULL << 32;          /* (2^32)^2 = 2^64 > x */
    while (low <= high)
    {
        uint64_t mid = low + ((high - low) >> 1);
        if (mid == 0 || mid <= x / mid)   /* avoid overflow of mid*mid */
            low = mid + 1;
        else
            high = mid - 1;
    }
    return (seq_t)(low - 1);
}

seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t limit1 = isqrt_u64((uint64_t)n);

    for (seq_t r1 = 0; r1 <= limit1; ++r1)
    {
        seq_t r1sq = r1 * r1;
        seq_t rest1 = (seq_t)n - r1sq;

        seq_t limit2 = isqrt_u64((uint64_t)rest1);

        for (seq_t r2 = 0; r2 <= limit2; ++r2)
        {
            seq_t r2sq = r2 * r2;
            seq_t rest2 = rest1 - r2sq;

            seq_t r3 = isqrt_u64((uint64_t)rest2);
            if (r3 * r3 == rest2)
            {
                int pos = (r1 > 0) + (r2 > 0) + (r3 > 0);
                seq_t factor = (seq_t)1 << pos;
                if (((r1 + r2 + r3) & 1) != 0)
                    factor = -factor;
                result += factor;
            }
        }
    }
    return result;
}

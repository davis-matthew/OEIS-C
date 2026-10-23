/*
 * A196440  a(n) = the sum of numbers k <= n such that GCQ_A(n, k) >= 2 (see
 *          definition in comments).
 * https://oeis.org/A196440
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 361, u32 n <= 1001, u64 n <= 1001,
 *            u128 n <= 1001, big any n
 * Helpers:   gcd_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A196440
 *  a(n) = ∑_{k≤n, GCQ_A(n,k)≥2} k
 *  GCQ_A(a,b) = greatest positive integer q≤min(a,b) that
 *  does NOT divide a and does NOT divide b.
 *  For n<1 the sequence is undefined; we return 0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd_uint64.h"

/* Euclidean gcd for uint64_t */

seq_t a(idx_t n)
{
    /* Reduce idx_t to native unsigned integer for loops */
    uint64_t un = (uint64_t)n;
    if (un < 2) return (seq_t)0;

    seq_t ans = 0;
    uint64_t L = 1;      /* lcm of numbers ≤ current k that do not divide n */
    bool overflow = false;

    for (uint64_t k = 2; k <= un; ++k)
    {
        /* Update L if k does not divide n and we haven’t overflowed yet */
        if (!overflow && (un % k != 0))
        {
            uint64_t g = gcd_uint64(L, k);
            __uint128_t t = ((__uint128_t)L / g) * k;
            if (t > (__uint128_t)un)
                overflow = true;          /* L will be > n hence > k for all future k */
            else
                L = (uint64_t)t;
        }

        bool gcq_nonzero;
        if (overflow)
        {
            gcq_nonzero = true;
        }
        else
        {
            if (L <= k && k % L == 0)
                gcq_nonzero = false;      /* GCQ_A(n,k) = 0 */
            else
                gcq_nonzero = true;       /* GCQ_A(n,k) ≥ 2 */
        }

        if (gcq_nonzero)
            ans += (seq_t)k;
    }

    return ans;
}

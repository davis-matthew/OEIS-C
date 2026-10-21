/*
 * A063827  a(n) = round(sqrt(a(n-2)^2 + a(n-1)^2)) with a(0) = 1 and a(1) =
 *          2.
 * https://oeis.org/A063827
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = round(sqrt(a(n-2)^2 + a(n-1)^2)) with a(0)=1, a(1)=2
 * Range:     u8 n <= 21, u16 n <= 44, u32 n <= 90, u64 n <= 182,
 *            u128 n <= 116, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == (idx_t)0) return (seq_t)1;
    if (n == (idx_t)1) return (seq_t)2;

    /* Variables for the two preceding terms */
    seq_t a_prev2 = (seq_t)1;
    seq_t a_prev1 = (seq_t)2;
    seq_t a_curr;

    /* If seq_t is small enough, use a 128‑bit type for the intermediate
       calculations; otherwise rely on the provided arbitrary‑precision
       sqrt function. */
    if (sizeof(seq_t) <= 8) {
        for (idx_t i = 2; i <= n; ++i) {
            /* Compute floor(sqrt(a_prev2^2 + a_prev1^2)) using binary search
               on a __uint128_t range. */
            __uint128_t u0 = (__uint128_t)a_prev2;
            __uint128_t u1 = (__uint128_t)a_prev1;
            __uint128_t sum = u0 * u0 + u1 * u1;

            __uint128_t low  = 0;
            __uint128_t high = ( (__uint128_t)(a_prev2 > a_prev1 ? a_prev2 : a_prev1) * 2 ) + 1;
            while (low < high) {
                __uint128_t mid = (low + high + 1) >> 1;
                if (mid * mid <= sum)
                    low = mid;
                else
                    high = mid - 1;
            }
            __uint128_t floorSq = low;
            __uint128_t diff = sum - floorSq * floorSq;
            /* The rounding threshold is (2*floorSq+1)/2.  To avoid fractions
               we compare 2*diff with (2*floorSq+1). */
            __uint128_t thresh = (floorSq << 1) + 1;
            if ((diff << 1) >= thresh)
                a_curr = (seq_t)(floorSq + 1);
            else
                a_curr = (seq_t)floorSq;

            a_prev2 = a_prev1;
            a_prev1 = a_curr;
        }
    } else {
        /* seq_t is big enough (e.g., arbitrary‑precision).  Use floor_sqrt
           from the utilities. */
        for (idx_t i = 2; i <= n; ++i) {
            seq_t sum = a_prev2 * a_prev2 + a_prev1 * a_prev1;
            seq_t s   = floor_sqrt(sum);
            seq_t diff = sum - s * s;
            seq_t thresh = (seq_t)2 * s + (seq_t)1;
            if ((seq_t)2 * diff >= thresh)
                a_curr = s + (seq_t)1;
            else
                a_curr = s;
            a_prev2 = a_prev1;
            a_prev1 = a_curr;
        }
    }
    return a_prev1;
}

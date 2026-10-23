/*
 * A186516  Adjusted joint rank sequence of (f(i)) and (g(j)) with f(i) after
 *          g(j) when f(i)=g(j), where f(i)=i^2 and g(j)=4+5j^2. Complement of
 *          A186515.
 * https://oeis.org/A186516
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + ⌊√(5*n² + 7/2)⌋ where n ≥ 1 (sequence A186516)
 * Range:     u8 n <= 79, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* 
       Compute m = ⌊√(5*n² + 7/2)⌋ using integer arithmetic.
       The inequality 2*m² ≤ 10*n² + 7 < 2*(m+1)²
       allows a binary search over m.
    */
    __uint128_t N   = ( __uint128_t )n * n;          /* n²   */
    __uint128_t RHS = 10 * N + 7;                   /* 10*n²+7 */

    __uint128_t lo   = 0;
    __uint128_t hi   = 3 * ( __uint128_t )n + 10;   /* safe upper bound */
    __uint128_t best = 0;

    while (lo <= hi)
    {
        __uint128_t mid = lo + ((hi - lo) >> 1);
        __uint128_t left = 2 * mid * mid;           /* 2*mid² */

        if (left <= RHS)
        {
            best = mid;
            lo   = mid + 1;
        }
        else
        {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }

    return (seq_t)n + (seq_t)best;
}

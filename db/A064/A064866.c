/*
 * A064866  Write numbers 1, then 1 up to 2^2, then 1 up to 3^2, then 1 up to
 *          4^2 and so on.
 * https://oeis.org/A064866
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 1495, u16 n <= 9214, u32 n <= 9214, u64 n <= 9214,
 *            u128 n <= 9214, big any n
 * Helpers:   sum_squares
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A064866: concatenation of the ranges 1..m² for m = 1,2,3,…  
 * Formula implemented: a(n) = n - S(m-1), where
 *          S(t) = t(t+1)(2t+1)/6      (sum of squares)
 * and    m = min{t : S(t) ≥ n}. */
#include <stdint.h>
#include "../utils/sum_squares.h"

seq_t a(idx_t n)
{
    /* Offset starts at 1; n == 0 is undefined, return 0 for safety. */
    if (n == 0) return 0;

    /* Find the smallest m such that S(m) ≥ n by binary search. */
    idx_t low  = 1;
    idx_t high = 1;
    __int128 target = n;

    /* Expand high until it is large enough. */
    while (sum_squares(high) < target) {
        high <<= 1;                 /* high increases by factor 2 */
    }

    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        if (sum_squares(mid) >= target)
            high = mid;
        else
            low = mid + 1;
    }

    idx_t       m     = low;
    __int128    prev  = sum_squares(m - 1);
    seq_t       result = (seq_t)(target - prev);   /* 0 < result ≤ m² ≤ 4·10¹² */
    return result;
}

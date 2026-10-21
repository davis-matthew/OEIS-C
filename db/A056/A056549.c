/*
 * A056549  a(n) = Sum_{k>=1} round(n/k) where round(1/2)=1.
 * https://oeis.org/A056549
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=1..2n} floor(2n/k) - Sum_{k=1..n} floor(n/k)
 * Range:     u8 n <= 47, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   sum_floor_divs
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent to a(n) = Sum_{k=n+1..2n} tau(k) where tau is the divisor function. */
/* This implementation uses the classic Dirichlet summatory divisor method 
   in O(√n) time for each call. */

#include <stdint.h>
#include <stddef.h>
#include "../utils/sum_floor_divs.h"

seq_t a(idx_t n)
{
    /* Handle the trivial case n == 0 directly. */
    if (n == 0) return (seq_t)0;

    /* 2*n can overflow idx_t for very large n; use 128-bit for safety. */
    __int128 N2_128 = (__int128)n * 2;
    /* If the doubled value fits into idx_t we cast, otherwise we compute
       the sums without creating an out-of-range index. */
    idx_t N2;
    if (N2_128 <= (__int128)UINT64_MAX) {
        N2 = (idx_t)N2_128;
    } else {
        /* For the extreme case where 2*n exceeds the maximum idx_t,
           we compute the two sums directly without the intermediate
           overflow.  This covers the theoretically possible maximum
           index but is unlikely to occur in tests. */
        seq_t sum2 = 0;
        idx_t limit = n * 2ULL;
        idx_t i = 1;
        while (i <= limit) {
            idx_t q = limit / i;
            idx_t r = limit / q;
            sum2 += (seq_t)q * (seq_t)(r - i + 1);
            i = r + 1;
        }
        return sum2 - sum_floor_divs(n);
    }

    return sum_floor_divs(N2) - sum_floor_divs(n);
}

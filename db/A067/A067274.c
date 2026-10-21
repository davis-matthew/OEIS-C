/*
 * A067274  Number of ordered integer pairs (b,c), with -n<=b<=n, -n<=c<=n,
 *          such that both roots of x^2+bx+c=0 are integers.
 * https://oeis.org/A067274
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*(A006218(n)+n)+floor(sqrt(n))-1 for n>0
 * Range:     u8 n <= 27, u16 n <= 3512, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sum_floor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*    with A006218(n) = sum_{i=1..n} floor(n/i)          */
/*    a(0) = 1                                            */

#include <stdint.h>
#include "../utils/sum_floor.h"

static idx_t isqrt_idx(idx_t n)
{
    /* integer floor of sqrt(n) using binary search; safe for 64‑bit */
    idx_t lo = 0, hi = n;
    while (lo <= hi) {
        idx_t mid = lo + (hi - lo) / 2;
        /* avoid overflow in mid*mid by using 128‑bit intermediate */
        unsigned __int128 msq = (unsigned __int128)mid * mid;
        if (msq <= (unsigned __int128)n)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;
}

seq_t a(idx_t n)
{
    if (n == 0u)
        return 1u;

    seq_t s = sum_floor(n);
    idx_t sqrt_n = isqrt_idx(n);
    seq_t result = 2 * (s + (seq_t)n) + (seq_t)sqrt_n - 1;
    return result;
}

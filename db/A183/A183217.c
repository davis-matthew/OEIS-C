/*
 * A183217  Complement of the pentagonal numbers.
 * https://oeis.org/A183217
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor(1/2 + sqrt(2n/3))
 * Range:     u8 n <= 242, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root for 64‑bit unsigned integers (floor value). */
static inline idx_t isqrt_u64(idx_t x)
{
    idx_t lo = 0, hi = x, mid, res = 0;
    while (lo <= hi) {
        mid = lo + ((hi - lo) >> 1);
        __uint128_t m2 = (__uint128_t)mid * mid;
        if (m2 == x) return mid;
        if (m2 < x) { res = mid; lo = mid + 1; }
        else       { hi = mid - 1; }
    }
    return res;
}

seq_t a(idx_t n)
{
    /* The sequence consists of all positive integers that are not  
       pentagonal numbers (p(m)=m(3m-1)/2).  We walk through the
       integers, skipping pentagonal ones, until we have counted
       n valid numbers. */
    idx_t k = 1;            /* current integer to test   */
    idx_t count = 0;        /* number of valid integers seen */

    while (count < n) {
        /* Check whether k is pentagonal: 1+24k must be a perfect
           square s*s and (s+1) divisible by 6. */
        idx_t s = isqrt_u64(1 + 24 * k);
        int is_pentagonal = 0;
        if (s * s == 1 + 24 * k && ((s + 1) % 6) == 0) {
            is_pentagonal = 1;
        }

        if (!is_pentagonal) {
            ++count;
            if (count == n) return (seq_t)k;
        }
        ++k;
    }

    /* Should never reach here for n >= 1 */
    return (seq_t)0;
}

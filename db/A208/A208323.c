/*
 * A208323  "Natural" rectangular pyramidal numbers.
 * https://oeis.org/A208323
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sum_{i=0}^{b-1} (a-i)*(b-i), where a*b = n and a >= b are
 *            chosen so that |a - b| is minimal (b is the largest divisor of n
 *            not exceeding sqrt(n))
 * Range:     u8 n <= 79, u16 n <= 3347, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root: floor(sqrt(n)). Requires only integer arithmetic and
   works for all 0 <= n <= 2^64-1. */
static inline idx_t isqrt(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if (mid <= n / mid)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    /* Step 1: find the largest divisor b of n that is <= sqrt(n). */
    idx_t root = isqrt(n);
    idx_t b = 1;
    for (idx_t i = root; i > 0; --i) {
        if (n % i == 0) {
            b = i;
            break;
        }
    }

    /* Step 2: compute a = n / b. */
    seq_t a_val = (seq_t)(n / b);
    idx_t bb = b;

    /* Step 3: compute the sum using the closed‑form expression to avoid
       loops.  All intermediate calculations are performed in 128‑bit
       arithmetic to prevent overflow when seq_t is 64 bits. */
    __int128 tmpA = (__int128)(a_val - bb) * (__int128)bb * (__int128)(bb + 1);
    tmpA /= 2;
    seq_t termA = (seq_t)tmpA;

    __int128 tmpB = (__int128)bb * (__int128)(bb + 1) * (__int128)(2 * bb + 1);
    tmpB /= 6;
    seq_t termB = (seq_t)tmpB;

    return termA + termB;
}

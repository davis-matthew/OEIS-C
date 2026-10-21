/*
 * A216279  Number of solutions to the equation x^2+5y^2 = n with x and y > 0.
 * https://oeis.org/A216279
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   number of solutions to x^2 + 5*y^2 = n with x > 0 and y > 0
 *            Computation: brute‑force search over y up to sqrt(n/5)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>          /* for uintptr_t if needed */

/* Integer square root for 64‑bit unsigned integers.
   Returns floor(sqrt(n)). */
static idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0;
    idx_t high = (idx_t)1ULL << 32;   /* sqrt(UINT64_MAX) <= 2^32 */

    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        /* avoid overflow: mid <= n/mid is equivalent to mid*mid <= n */
        if (mid == 0 || mid <= n / mid)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    seq_t count = 0;

    /* y must be positive.  For each y, check if n – 5y² is a perfect square. */
    for (idx_t y = 1;; ++y) {
        idx_t val = 5 * y * y;
        if (val > n)
            break;
        idx_t rem = n - val;
        if (rem == 0)            /* x would be 0, not allowed */
            continue;

        idx_t x = isqrt_u64(rem);
        if (x * x == rem)        /* perfect square found, x > 0 automatically */
            ++count;
    }

    return count;
}

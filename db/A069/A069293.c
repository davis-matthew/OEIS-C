/*
 * A069293  Sum of square divisors of n <= sqrt(n).
 * https://oeis.org/A069293
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{d|n, d is a perfect square AND d ≤ sqrt(n)} d
 * Range:     u8 n <= 9555, u16 n <= 9555, u32 n <= 9555, u64 n <= 9555,
 *            u128 n <= 9555, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* integer square root (floor) for 64‑bit values, using binary search */
static idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0, high = n, mid, res = 0;
    while (low <= high) {
        mid = low + ((high - low) >> 1);
        /* use 128‑bit to avoid overflow when mid*mid is computed */
        __int128 prod = (__int128)mid * mid;
        if (prod <= n) {
            res = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return res;
}

/* main function for OEIS A069293 */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n == 0, but return 0 */
    /* limit = floor( sqrt( sqrt(n) ) ) = floor( n^(1/4) ) */
    idx_t sqrt_n   = isqrt_u64(n);
    idx_t limit    = isqrt_u64(sqrt_n);   /* floor(n^(1/4))   */
    seq_t sum = 0;

    for (idx_t t = 1; t <= limit; ++t) {
        idx_t t2 = t * t;                 /* t^2  (fits in idx_t)   */
        if (n % t2 == 0) {
            sum += (seq_t)t2;
        }
    }
    return sum;
}

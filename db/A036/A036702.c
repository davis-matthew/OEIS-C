/*
 * A036702  a(n)=number of Gaussian integers z=a+bi satisfying |z|<=n, a>=0,
 *          0<=b<=a.
 * https://oeis.org/A036702
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∑_{a=0}^{n} ( floor(√(n² – a²)) + 1 ) (with b≤a)
 * Range:     u8 n <= 24, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of Gaussian integers a+bi with |z|≤n, a≥0, 0≤b≤a. */

#include <stdint.h>

/* Integer square‑root for a 64‑bit value, returning the floor of √x.  */
static inline idx_t
isqrt_u64(idx_t x)
{
    if (x == 0 || x == 1)
        return x;
    idx_t low  = 0;
    idx_t high = (x >> 1) + 1;           /* √x ≤ x/2 + 1 for x≥2  */
    idx_t ans  = 0;
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        idx_t sq  = mid * mid;
        if (sq == x)
            return mid;
        if (sq < x) {
            ans = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    seq_t count = 0;
    idx_t n2 = n * n;
    for (idx_t a = 0; a <= n; ++a) {
        idx_t diff   = n2 - a * a;           /* n² – a²  (non‑negative)   */
        idx_t max_b  = isqrt_u64(diff);      /* floor √(n² – a²)           */
        if (max_b > a)
            max_b = a;                        /* enforce b ≤ a              */
        count += (seq_t)(max_b + 1);          /* b runs from 0 to max_b     */
    }
    return count;
}

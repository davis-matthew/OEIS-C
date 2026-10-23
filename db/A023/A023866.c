/*
 * A023866  a(n) = 1*t(n) + 2*t(n-1) + ... + k*t(n+1-k), where
 *          k=floor((n+1)/2) and t is A000201 (lower Wythoff sequence).
 * https://oeis.org/A023866
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1*t(n) + 2*t(n-1) + … + k*t(n+1-k) (k=floor((n+1)/2))
 *            where t(m) = A000201(m) = floor(m*phi) (phi = (1+√5)/2)
 * Range:     u8 n <= 11, u16 n <= 77, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Integer square root: floor(sqrt(x)) for non‑negative x. */
static idx_t integer_sqrt(idx_t x)
{
    idx_t lo = 0, hi = x;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        idx_t sq  = mid * mid;
        if (sq == x)
            return mid;
        if (sq < x)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;
}

/* Lower Wythoff sequence value t(m) = floor(m*phi) =
   ((m + floor(m*√5)) / 2). */
static seq_t lower_wythoff(idx_t m)
{
    /* compute floor(m*√5) = floor(√(5*m*m)) */
    idx_t temp = m * m;
    temp = temp * 5;
    idx_t sqrt5m = integer_sqrt(temp);
    return ((seq_t)m + (seq_t)sqrt5m) / 2;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* offset is 1, but guard against 0 */

    idx_t k   = (n + 1) / 2;
    seq_t sum = 0;

    /* Compute the weighted sum of the last k lower Wythoff numbers. */
    for (idx_t i = 1; i <= k; ++i) {
        idx_t m    = n + 1 - i;               /* index of t */
        seq_t tval = lower_wythoff(m);
        sum += (seq_t)i * tval;
    }
    return sum;
}

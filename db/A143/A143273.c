/*
 * A143273  Triangle read by rows: A127648 * A000012 * A130209.
 * https://oeis.org/A143273
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = r * d(k) where r = ceil( (sqrt(1+8*n) - 1) / 2 ), k = n -
 *            r*(r-1)/2, d(k) = number of divisors of k
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* floor square root of an unsigned __int128 value, result fits in idx_t */
static idx_t floor_sqrt_u128(unsigned __int128 x)
{
    unsigned __int128 lo = 0, hi = ((unsigned __int128)1) << 64; /* > sqrt(max) */
    while (lo < hi) {
        unsigned __int128 mid = (lo + hi + 1) >> 1;
        if (mid * mid <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return (idx_t)lo; /* fits in 64 bits because max sqrt < 2^64 */
}

/* number of divisors of k (τ(k)) */
static seq_t tau(idx_t k)
{
    seq_t res = 1;
    idx_t n = k;
    for (idx_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            idx_t exp = 0;
            while (n % p == 0) {
                n /= p;
                ++exp;
            }
            res *= (seq_t)(exp + 1);
        }
    }
    if (n > 1)
        res *= 2; /* remaining prime factor */
    return res;
}

seq_t a(idx_t n)
{
    /* sequence is defined for n>=1; return 0 for n==0 as a safe fallback */
    if (n == 0)
        return 0;

    /* compute r: smallest integer with r*(r+1)/2 >= n */
    unsigned __int128 val = (unsigned __int128)1 + (unsigned __int128)8 * n;
    idx_t sqrt_val = floor_sqrt_u128(val);
    idx_t r = (sqrt_val - 1) / 2; /* floor of (-1+sqrt)/2 */
    /* adjust if needed */
    if ( ((unsigned __int128)r * (r + 1)) / 2 < n )
        ++r;

    /* compute k = n - prev_total, prev_total = r*(r-1)/2 */
    unsigned __int128 prev_total = ((unsigned __int128)r * (r - 1)) / 2;
    idx_t k = n - (idx_t)prev_total; /* 1 <= k <= r */

    /* return r * τ(k) */
    seq_t divs = tau(k);
    return (seq_t)r * divs;
}

/*
 * A089734  Smallest b (b >=0) such that 6*n+1= p + 2*b^2, p prime.
 * https://oeis.org/A089734
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest b ≥ 0 such that 6*n+1 = p + 2*b² with p prime
 * Range:     u8 n <= 23, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* --------------------------------------------------------- */
/* Helper functions for integer arithmetic on seq_t */
#include <stdbool.h>

/* Integer square root for seq_t (generic implementation) */
static seq_t isqrt_seq(seq_t n)
{
    seq_t low = 0;
    seq_t high = n;
    seq_t mid, mid2;
    while (low <= high)
    {
        mid  = (low + high) >> 1;
        mid2 = mid * mid;
        if (mid2 == n)
            return mid;
        if (mid2 < n)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

/* Prime test for seq_t using trial division on odd divisors 3,5,7,... */
static bool is_prime_seq(seq_t x)
{
    if (x < 2)
        return false;
    if (x == 2 || x == 3)
        return true;
    if ((x & 1) == 0) /* even and >2 */
        return false;

    seq_t sqrtx = isqrt_seq(x);
    for (seq_t d = 3; d <= sqrtx; d += 2)
    {
        if (x % d == 0)
            return false;
    }
    return true;
}

/* --------------------------------------------------------- */
/* Main sequence function */
seq_t a(idx_t n)
{
    /* Convert to seq_t early to avoid overflow on small integer types       */
    seq_t T = (seq_t)6 * n + (seq_t)1; /* 6*n + 1 */

    seq_t b = 0;
    while (true)
    {
        seq_t two_b2 = (seq_t)2 * b * b;
        /* If 2*b² > T-2, further b would make p < 2; stop searching.          */
        if (two_b2 > T - (seq_t)2)
            break;

        seq_t p = T - two_b2;
        if (p >= (seq_t)2 && is_prime_seq(p))
            return b;            /* found minimal b */

        ++b;
    }

    /* In theory a solution always exists; return 0 as a fallback.          */
    return (seq_t)0;
}

/*
 * A051506  a(n) = min { n^n, floor(100/sqrt(n)) }.
 * https://oeis.org/A051506
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min{ n^n , floor(100 / sqrt(n)) }
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses exact integer arithmetic; no floating-point operations are employed. */
#include <stdint.h>

static unsigned long long isqrt_u64(unsigned long long x)
{
    unsigned long long lo = 0;
    unsigned long long hi = 1ULL << 32;          /* hi is large enough for all possible x */
    while (lo < hi)
    {
        unsigned long long mid = (lo + hi + 1) >> 1;
        if (mid * mid <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    /* n >= 1 by definition of the sequence */
    uint64_t div = 10000ULL / (uint64_t)n;          /* 100^2 / n  */
    uint64_t k_int = isqrt_u64(div);               /* floor( sqrt(10000 / n) ) == floor(100 / sqrt(n)) */
    seq_t k = (seq_t)k_int;

    /* Compute n^n but stop once the value exceeds k. */
    unsigned __int128 pow = 1;
    for (idx_t i = 1; i <= n; ++i)
    {
        pow *= (unsigned __int128)n;
        if (pow > k)          /* once greater than k, min will be k */
            return k;
    }
    return (seq_t)pow;        /* n^n <= k for very small n (1,2,3) */
}
